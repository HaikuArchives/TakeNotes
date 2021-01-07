/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 *
 * Last revision: Ilio Catallo, 26th June 2009
 *
 * Description: This is the application, with all the functions implemented.
 *				This is the main container of the functions related to an application.
 */

// Our Libraries
#include "NoteApplication.h"

// Libraries
#include <Alert.h>
#include <AppFileInfo.h>
#include <Autolock.h>
#include <Bitmap.h>
#include <Deskbar.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <IconUtils.h>
#include <Mime.h>
#include <Node.h>
#include <Path.h>
#include <Resources.h>
#include <Roster.h>
#include <Volume.h>
#include <VolumeRoster.h>
#include <Catalog.h>
#include <TranslationUtils.h>

#include <kernel/fs_index.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

// Constants
#define COLOR_CHANGED	'ccrq'
#define FONT_BOLD 		'fntb'
#define NEW				'new'
#define OPEN			'open'
#define DELETE			'dele'

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "NoteApplication"

// Global variables
NoteApplication *note_app;

//Useful variables (we don't want to repeat these hard to remember strings everywhere!)
const char*	kSignature = "application/x-vnd.ccc-TakeNotes";
const char*	kDeskbarSignature = "application/x-vnd.Be-TSKB";
const char*	kDeskbarItemName = "TakeNotes";


/*
* This function fills the image_info image struct with the app image of itself
* and eventually return B_OK
*/
status_t our_image(image_info& image){

	// Variables
	int32 cookie = 0;

	while (get_next_image_info(B_CURRENT_TEAM, &cookie, &image) == B_OK) {

		if ((char *)our_image >= (char *)image.text
			&& (char *)our_image <= (char *)image.text + image.text_size)
			return B_OK;
	}

	return B_ERROR;
}

// Constructor
NoteApplication :: NoteApplication()
			    : BApplication("application/x-vnd.ccc-TakeNotes"){

	// Check (and install) the MIME type
	CheckMime();

	/*
	* Creation of the directory with the association between notes
	* and applications
	*/
	DIR *dir;
	dir = opendir("/boot/home/config/settings/TakeNotes");
	if (dir == NULL)
		mkdir ("/boot/home/config/settings/TakeNotes", O_CREAT);

	// Private data members initialization
	fWindowCount = 0;
	note_app = this;
	fSettingsMessage = new BMessage();

	printf("%s\n",load_settings(fSettingsMessage, "settings", "TakeNotes"));

	//thaflo 2020: global settings
	// Open the config file, if it doesn't exist we create it
	BFile settings;
	status_t err;
	if ((err = settings.SetTo("/boot/home/config/settings/TakeNotes/settings", B_READ_WRITE | B_CREATE_FILE)) != B_OK){
		printf("%s\n",err);
	}


	// Create the file open panel
	fOpenPanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL, B_FILE_NODE, true,
		NULL, &fNoteRefFilter);
}

// Destructor
NoteApplication::~NoteApplication()
{
	delete fOpenPanel;
}

// Function ReadyToRun
void NoteApplication :: ReadyToRun(){

	// Variables
	BDeskbar	deskbar;
	BAlert 		*alert;
	short int	erg;

	// Check if the replicant isn't already installed in the Deskbar, avoid to ask if we already opened a note
	if (!deskbar.HasItem(kDeskbarItemName) && fWindowCount == 0){

		alert = new BAlert("", B_TRANSLATE("Do you want TakeNotes to live in the Deskbar?"), B_TRANSLATE("Never"), B_TRANSLATE("No thanks"), B_TRANSLATE("Install"), B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert -> SetShortcut(0, B_ESCAPE);
		erg = alert -> Go();
		// In case we have to install it in the deskbar...
		if (erg == 2) {
			_InstallReplicantInDeskbar();
			return;
		}
		if (erg == 1) {
			fSettingsMessage -> SetBool("live_in_deskbar", false);
			save_settings(fSettingsMessage, "settings", "TakeNotes");
		}
	}

	// If there's some window opened return
	if (fWindowCount > 0)
		return;

	// If there aren't windows opened, we open a note
	// thaflo 2020: load defaults and last used note
	//fSettingsMessage = new BMessage();
	entry_ref *fRef = new entry_ref;

	if(load_settings(fSettingsMessage, "settings", "TakeNotes") != B_OK) {
		OpenNote();
	} else {
		if(fSettingsMessage -> FindBool("load_last_note") == TRUE)
			fSettingsMessage -> FindRef("last_note", fRef);
	}

	OpenNote(fRef);
}

// Function CheckMime
status_t NoteApplication :: CheckMime(){

	// Variables
	BMimeType 	takenotes("application/takenotes");

	image_info		appInfo;
	entry_ref		ref;
	BMessage	 	msg;
	size_t			size;
	const void		*data = NULL;
	BFile			file;
	BResources		resource;
	app_info 		appInfo2;
	BFile			file2;
	BAppFileInfo	appFileInfo;

	BVolumeRoster	volumeRoster;
	BVolume 		rootVolume;

	// We define the extensions for the mime type
	msg.AddString("extensions","tkn");

	// Create a BAppFileInfo object linked to the application itself
	be_app->GetAppInfo(&appInfo2);
	file2.SetTo(&appInfo2.ref, B_READ_WRITE);
	appFileInfo.SetTo(&file2);

	// Grab the file icon stored in the resources
	if (our_image(appInfo) != B_OK)
		return B_ERROR;

	file.SetTo(appInfo.name, B_READ_ONLY);

	if (resource.SetTo(&file) != B_OK)
		return B_ERROR;

	// Load the icon as a blob from the resource
	data = resource.LoadResource(B_VECTOR_ICON_TYPE,2,&size);

	if (data != NULL){
		// Set the icon for the mimetype
		appFileInfo.SetIconForType(takenotes.Type(), (uint8 *)data, size);

	} else
		return B_ERROR;

	appFileInfo.SetSupportedTypes(&msg);

	// Check if the mimetype is installed or not
	if (takenotes.InitCheck() == B_OK){


		// Check if the mimetype is already installed in the DB
		if (takenotes.IsInstalled()){

			return B_OK;
		}

		// First of all we install the TakeNotes MIME type
		if (takenotes.Install() != B_OK)
				return B_ERROR;


		if (takenotes.SetFileExtensions(&msg) != B_OK){
				return B_ERROR;

		}

		// Set the preferred application
		if (takenotes.SetPreferredApp("application/x-vnd.ccc-TakeNotes") != B_OK){
				return B_ERROR;
		}

		// Create the index structure for the extra attributes
		volumeRoster.GetBootVolume(&rootVolume);

		fs_create_index(rootVolume.Device(),"TAKENOTES:tagone",B_STRING_TYPE, 0);
		fs_create_index(rootVolume.Device(),"TAKENOTES:tagtwo",B_STRING_TYPE, 0);
		fs_create_index(rootVolume.Device(),"TAKENOTES:tagthree",B_STRING_TYPE, 0);

		// Add some extra attributes in order to manage custom tags
		BMessage attr;

		attr.AddString("attr:name","TAKENOTES:tagone");
		attr.AddString("attr:name","TAKENOTES:tagtwo");
		attr.AddString("attr:name","TAKENOTES:tagthree");

		attr.AddString("attr:public_name","tagone");
		attr.AddString("attr:public_name","tagtwo");
		attr.AddString("attr:public_name","tagthree");

		attr.AddInt32("attr:type",B_STRING_TYPE);
		attr.AddInt32("attr:type",B_STRING_TYPE);
		attr.AddInt32("attr:type",B_STRING_TYPE);

		attr.AddInt32("attr:width",60);
		attr.AddInt32("attr:width",60);
		attr.AddInt32("attr:width",60);
		attr.AddInt32("attr:width",60);

		attr.AddBool("attr:viewable",true);
		attr.AddBool("attr:viewable",true);
		attr.AddBool("attr:viewable",true);

		attr.AddBool("attr:editable",true);
		attr.AddBool("attr:editable",true);
		attr.AddBool("attr:editable",true);

		attr.AddBool("attr:extra",true);
		attr.AddBool("attr:extra",true);
		attr.AddBool("attr:extra",true);

		attr.AddInt32("attr:alignment",0);
		attr.AddInt32("attr:alignment",0);
		attr.AddInt32("attr:alignment",0);

		if (takenotes.SetAttrInfo(&attr) != B_OK){
			return B_ERROR;
		}

		// Add a short description for the MIME type
		if (takenotes.SetShortDescription("TakeNotes") != B_OK)
			return B_ERROR;

		if (takenotes.SetLongDescription("TakeNotes post-it application") != B_OK)
			return B_ERROR;

		// If we arrived here, it should be all okay!
		return B_OK;
	}
	else
		return B_ERROR;
}

// Function used to open a note, counting how many notes are opened
void
NoteApplication::OpenNote(entry_ref *ref)
{
	if (ref == NULL) {
		new NoteWindow();
		fWindowCount++;
		return;
	}

	entry_ref linkedRef;
	BEntry entry(ref, true);
	if(entry.InitCheck() != B_OK || !entry.Exists() ||
			entry.GetRef(&linkedRef) != B_OK) {
		new NoteWindow();
		fWindowCount++;
		return;
	}

	for (int32 i = 0; i < CountWindows(); i++) {
		NoteWindow *noteWin = dynamic_cast<NoteWindow*>(WindowAt(i));
		if (noteWin == NULL || noteWin->fRef == NULL ||
			*(noteWin->fRef) != linkedRef)
			continue;

		noteWin->Activate();
		return;
	}

	new NoteWindow(&linkedRef);

	fWindowCount++;
}

// Function used to close a note
void NoteApplication :: CloseNote(){

	fWindowCount--;
	if (fWindowCount == 0){
		BAutolock lock(this);
		Quit();
	}

}

// Function that is activated when we receive some arguments
void NoteApplication :: ArgvReceived(int32 argc, char** argv){

	// Variables
	const 		char 		*cwd = "";
	BMessage 	*message = CurrentMessage();
	fSettingsMessage = new BMessage();

	// Extract the cwd (current working directory)
	if (message != NULL){

		if (message->FindString("cwd",&cwd) != B_OK)
			cwd = "";
	}

	/*
	* Check if it is an absolute or relative path
	* If the path is a relative one we make it absolute
	* Eventually we open the note(s)
	*/
	for (int i=1; i < argc; i++){

		// Variables
		BPath path;
		entry_ref ref;

		if (argv[i][0] == '/'){
			path.SetTo(argv[i]);
		} else {
			path.SetTo(cwd, argv[i]);
		}

		get_ref_for_path(path.Path(), &ref);
		// Open a note
		entry_ref *fRef = new entry_ref;
		fRef = &ref;

		// load settings so we can set the appropriate mark on the "open last note on startup" menu
		if (load_settings(fSettingsMessage, "settings", "TakeNotes") != B_OK)
			printf("couldn't load settings\n");

		OpenNote(&ref);
	}
}

// Function RefsReceived
void NoteApplication :: RefsReceived(BMessage *message){
	// Variables
	int32 index = 0;
	entry_ref ref;
	fSettingsMessage = new BMessage();

	if (load_settings(fSettingsMessage, "settings", "TakeNotes") != B_OK)
			printf("couldn't load settings\n");

	while(message->FindRef("refs", index++, &ref) == B_OK){
		OpenNote(&ref);
	}
}

// Function used to receive messages
void NoteApplication :: MessageReceived(BMessage *message){
	// Search if the message that was caputed is handled by the application
	switch(message->what){

		case OPEN:
			fOpenPanel->Show();
		break;

		case DELETE:
			printf("todo: delete note\n");
		break;

		case NEW:
			new NoteWindow();
			fWindowCount++;
		break;

		case B_SILENT_RELAUNCH:
			OpenNote();
		break;

		default:
			BApplication::MessageReceived(message);
		break;
	}
}

/*
* Function used to install the replicant in the deskbar
* The replicant will act as the controller of the application
*/
void NoteApplication :: _InstallReplicantInDeskbar(){

	// Variables
	image_info  info;
	entry_ref	ref;

	// If it succeded in finding itself
	if (our_image(info) == B_OK && get_ref_for_path(info.name, &ref) == B_OK){

		// Find itself in the file system
		BPath path(&ref);

		// Initialize the deskbar object
		BDeskbar deskbar;

		// If the deskbar is running we finally install
		if (!deskbar.IsRunning())
			return;

		if (deskbar.AddItem(&ref) != B_OK)
			return;

	} else
		return;

	/*
	* Quit the application, an indipendent instance of TakeNotes is now running
	* as a replicant in the Deskbar
	*/
	Quit();
}

// Main
int main(){

	NoteApplication myApp;

	myApp.Run();
	return(0);
}


//stolen from LaunchBox->support.cpp
//thanks Stippi!

status_t
load_settings(BMessage* message, const char* fileName, const char* folder)
{
	status_t ret = B_BAD_VALUE;
	if (message) {
		BPath path;
		if ((ret = find_directory(B_USER_SETTINGS_DIRECTORY, &path)) == B_OK) {
			// passing folder is optional
			if (folder)
				ret = path.Append( folder );
			if (ret == B_OK && (ret = path.Append(fileName)) == B_OK ) {
				BFile file(path.Path(), B_READ_ONLY);
				if ((ret = file.InitCheck()) == B_OK) {
					ret = message->Unflatten(&file);
					file.Unset();
				}
			}
		}
	}
	return ret;
}


status_t
save_settings(BMessage* message, const char* fileName, const char* folder)
{
	status_t ret = B_BAD_VALUE;
	if (message) {
		BPath path;
		if ((ret = find_directory(B_USER_SETTINGS_DIRECTORY, &path)) == B_OK) {
			// passing folder is optional
			if (folder && (ret = path.Append(folder)) == B_OK)
				ret = create_directory(path.Path(), 0777);
			if (ret == B_OK && (ret = path.Append(fileName)) == B_OK) {
				BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE
					| B_ERASE_FILE);
				if ((ret = file.InitCheck()) == B_OK) {
					ret = message->Flatten(&file);
					file.Unset();
				}
			}
		}
	}
	return ret;
}
