/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Ilio Catallo
 *			Stefano Celentano
 *			Eleonora Ciceri
 *
 * Last revision: thaflo, december 2020
 *
 * Description: View of the note. It can be implemented in three instances:
 *				- the view in the deskbar is the controller of the application
 *				- the view in the window, that is the container of the NoteText
 *				- the view as a replicant placed on a shelf (eg: the desktop)
 */

// Our libraries
#include "NoteApplication.h"
#include "NoteView.h"
#include <private/interface/AboutWindow.h>

// Other libraries
#include <Application.h>
#include <Alert.h>
#include <Deskbar.h>
#include <Dragger.h>
#include <File.h>
#include <FilePanel.h>
#include <IconUtils.h>
#include <MenuItem.h>
#include <NodeInfo.h>
#include <PopUpMenu.h>
#include <String.h>
#include <Resources.h>
#include <Roster.h>

//translation
#include <Catalog.h>
#include <TranslationUtils.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "NoteView"

// System libraries
#include <stdio.h>
#include <stdlib.h>

// Messages
#define FONT_BOLD 			'fntb'
#define CHOOSE_APPL			'cspp'
#define	OPEN_TAKENOTES		'onwp'
#define	OPEN_FILE			'onfl'
#define REMOVE_ASSOCIATION	'rmsc'
#define SETTINGS_CLOSE		'settc'
#define OPEN_NEW_NOTE		'opnn'

// Constructor
NoteView :: NoteView(BRect frame, int32 resizingMode, bool inDeskbar, BHandler *handler)
	   	 : BView(frame, "TakeNotes", resizingMode, B_FRAME_EVENTS | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	   		fInDeskbar(inDeskbar){

	// Variable
	BDragger *dragger;

	// Initilization
	fBitmap = NULL;
	fHash = new AppHashTable();

	if (handler)
		fMessenger = new BMessenger(handler);

	// Graphical Stuff
	if (!inDeskbar){

		// We don't have to add the dragger if the view is in the deskbar
		SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		BRect rect(Bounds());
		rect.left = rect.right - 7;
		rect.top = rect.bottom - 7;
		dragger = new BDragger(rect, this, B_FOLLOW_BOTTOM | B_FOLLOW_RIGHT);
		AddChild(dragger);
		dragger->SetViewColor(ViewColor());

	} else {

		fReplicated = false;
		printf("sono nella deskbar\n");
		InitBitmap();
	}
}

/*
* Costructor by message
* We can restore a NoteText starting from a BMessage that contains its structure
*/
NoteView :: NoteView (BMessage *msg, BHandler *handler)
		   : BView(msg){

	// Variables
	app_info	info;

	// Initialization
	fReplicated = true;
	if (handler)
		fMessenger = new BMessenger(handler);

	// Check if we are in the deskbar
	if (be_app->GetAppInfo(&info) == B_OK && !strcasecmp(info.signature, "application/x-vnd.Be-TSKB")){
		fInDeskbar = true;
		fReplicated = false;
	} else {
		printf("%s\n",info.signature);
	}
}

// Destructor (actually empty, no dynamic object to destory here)
NoteView :: ~NoteView(){}

// Function used to draw in the view
void NoteView :: Draw (BRect update){

	printf("fReplicated: %d\n",fReplicated);

	if (fReplicated || !fInDeskbar)
		return;

	/*
	* We have to write the TakeNotes icon in the deskbar so we
	* set the drawing mode to B_OP_ALPHA to obtain the icon background
	* alpha blended
	*/
	SetDrawingMode(B_OP_ALPHA);
	DrawBitmap(fBitmap);
	SetDrawingMode(B_OP_COPY);
}

// Function InitBitmap
void NoteView :: InitBitmap(){

	// Variables
	image_info 	info;
	BFile		file;
	const 		void*		data = NULL;
	size_t		size;
	BBitmap 	*icon;

	// Initialization
	if (fBitmap){
		delete fBitmap;
	}

	fBitmap = NULL;

	// Try to locate ourselves on the FS and open in read only
	if (our_image(info) != B_OK)
		return;

	file.SetTo(info.name,B_READ_ONLY);
	if (file.InitCheck() < B_OK)
		return;

	// Loading the resourced linked in the binary
	BResources resources(&file);
	if (resources.InitCheck() < B_OK)
		return;

	// Load the vectorial icon into a void pointer
	data = resources.LoadResource(B_VECTOR_ICON_TYPE, 2, &size);
	if (data != NULL){

		// Obtain the icon from the blob
		icon = new BBitmap(Bounds(), B_RGBA32);

		if (icon->InitCheck() == B_OK && BIconUtils::GetVectorIcon((const uint8 *)data,size,icon) == B_OK){
			// We succeed in creating the icon so we can associate it to fBitmap and invalidate the view
			fBitmap = icon;
			Invalidate();
		} else {
			// Some errors occured so we delete the icon
			delete icon;
		}
	} else {
		// No B_VECTOR_ICON_TYPE data in the application resource
		return;
	}
}

/*
* Hook function-override, when the view is attached to a parent window we
* find if this parent is the deskbar or the NoteWindow
*/
void NoteView :: AttachedToWindow(){

	// Actually execute the real code
	BView :: AttachedToWindow();

	/*
	* if there's a parent it means we're in the deskbar
	* so we set the background color the same as the deksbar
	* in order to obtain the alpha trasparency for the icon
	*/
	if (Parent() && !fReplicated){
		SetViewColor(Parent()->ViewColor());
		SetLowColor(ViewColor());

	}
}

/*
* Hook function-override, it reacts when the user clicks on the view
* First it tries to find the role of the view (if it is in a replicant,
* in a NoteWindow, in the Deskbar).
* If it is in the deskbar, it will show the contextual menu with the
* note/application associations.
*
*/
void NoteView :: MouseDown(BPoint point){

	// Variables
	BList 		*aList;
	app_info 	*appInfo;
    int 		count;
    entry_ref	ref;
    BString		name;
    BPopUpMenu  *menu;
    int 		countSignatures;
    char 		*sig,
    			*note;
    BMenu 		*subMenu,
    			*noteMenu;
    BMessage 	*mess,
    			*messRemove;


	// If we're in Deskbar and we aren't a replicated note
	if (fInDeskbar && !fReplicated) {

		// Load the database of notes
		fHash = new AppHashTable();
		_LoadDB();

		// Create the menu
		menu = new BPopUpMenu(B_EMPTY_STRING, false, false);
		menu->SetAsyncAutoDestruct(true);
		menu->SetFont(be_plain_font);

		// Fill the menu
		menu->AddItem(new BMenuItem(B_TRANSLATE("Open TakeNotes" B_UTF8_ELLIPSIS),new BMessage(OPEN_TAKENOTES)));
		menu->AddItem(new BMenuItem(B_TRANSLATE("About" B_UTF8_ELLIPSIS), new BMessage(B_ABOUT_REQUESTED)));
		menu->AddItem(new BMenuItem(B_TRANSLATE("Quit"), new BMessage(B_QUIT_REQUESTED)));

		if (fHash->HasElement()) menu->AddSeparatorItem();

		// Initialization
    	aList = new BList;
    	appInfo = new app_info();

	   	// Obtaining the applications that are running
   		be_roster->GetAppList(aList);

		// We look for the current instances that are running
		for (count=0;count< aList->CountItems();count++){
   	    	team_id who = (team_id)(addr_t)aList->ItemAt(count);
   	  		be_roster->GetRunningAppInfo(who,appInfo);

       		// Show the name of the running program istead of its signature
        	ref = appInfo->ref;
   			name.SetTo(ref.name);

   			// Not show the system process like the *_server program or the Deskbar / Tracker
   			if (name.Compare("registrar") != 0 && name.Compare("Tracker") != 0 && name.Compare("Deskbar") != 0 &&
   		 		name.Compare("net_server") != 0 && name.Compare("syslog_daemon") !=0 && name.Compare("input_server") != 0 &&
   		 		name.Compare("midi_server") !=0 && name.Compare("print_server") !=0 && name.Compare("media_server") != 0 &&
   		 		name.Compare("media_addon_server") != 0 && name.Compare("afp_server") != 0 && name.Compare("debug_server") != 0) {

   		 		// Obtain the signature number
   				countSignatures = fHash -> GetNumSignatures();

   				// For each signature in the list we check if it is the hash table and in case we add it to the menu
   				for (int i = 0; i < countSignatures; i++) {
   					sig = fHash -> GetSignature(i);
   					if (strcmp (appInfo -> signature, sig) == 0) {
   						subMenu = new BMenu (ref.name);

   						// Make the list of the notes releated to that application
   						int countNotes = fHash -> GetNumNotes(sig);
   						for (int j = 0; j < countNotes; j++) {

   							// Obtain the current note path
   							note = fHash -> GetNote(sig, j);

   							// Create the list of notes for a given application
   							mess = new BMessage(OPEN_FILE);
   							mess -> AddString("Note", note);

   							messRemove = new BMessage (REMOVE_ASSOCIATION);
   							messRemove -> AddString ("Note", note);
   							messRemove -> AddString ("Signature", sig);

   							// Sub-menu useful to open or remove a note
   							noteMenu = new BMenu (note);
   							noteMenu -> AddItem (new BMenuItem ("Open", mess));
   							noteMenu -> AddItem (new BMenuItem ("Remove the association", messRemove));
   							noteMenu -> SetTargetForItems(this);
   							subMenu -> AddItem (noteMenu);
   						}

   						subMenu -> SetTargetForItems(this);
   						menu -> AddItem (subMenu);
   					}
   				}

   			}
     	}

		menu->SetTargetForItems(this);

		ConvertToScreen(&point);
		menu->Go(point, true, true, true);
	}
}

/*
* Function that is called when the NoteView is archived as a part of a BMessage
* this situation can happen 3 times: 	- the note is dehydrated (replicant on the desktop)
*										- the note is saved
*										- the application lives in the deskbar
*/

status_t NoteView :: Archive (BMessage *msg,bool deep) const{

	// Variables
	BFile 		file;
	BNodeInfo 	nodeinfo;
	BString		string;
	entry_ref	ref;
	status_t	err;
	const char	*name;
	BDirectory	dir;

	BView ::Archive(msg,deep);

	/*
	* Flags the message with the information regarding the application and the class name
	* This informations are required and they will be used by the costructor by message
	* to rehydrate the replicant
	*/
	msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
	msg->AddString("class","NoteView");

	// If the function has been called by NoteWindow::Save we should find this information and use them to save
	if (msg->FindRef("directory",&ref) == B_OK && msg -> FindString("name", &name) == B_OK) {

		dir.SetTo(&ref);
		if ((err = dir.InitCheck()) != B_OK)
			return err;

		// Create the note on the FS
		file.SetTo(&dir, name, B_READ_WRITE | B_CREATE_FILE);

		// File the note with the current view
		msg->Flatten(&file);

		// Add the MIME type information
		nodeinfo.SetTo(&file);
		nodeinfo.SetType("application/takenotes");

		//Deallocate the BFile object
		file.Unset();

	}

	return B_OK;
}

/*
* This function is called when we restore the note from a BMessage
* so it's called 3 times: 	- when we rehydrated a replicant
*							- when we restore a saved note
*							- when we install the application in the Deskbar
*/
BArchivable* NoteView :: Instantiate(BMessage *msg){

		return new NoteView(msg);

}

/*
* Functions that save to FS the associations between applications and notes
* It saves the whole Hash table structure to FS with the syntax:
*
* 		NotePath:ApplicationSignature
*
* This function is called when the user decides to remove an association so we need
* to update the informations stored in the config file.
*/

status_t NoteView :: _SaveDB(){

	// Variable
	BFile		config;
	BEntry		entry;
	BPath		path;
	status_t	err;
	entry_ref	ref;
	BString		toWrite;
	int			countSignatures,
				countNotes;

	// Open the config file in RW, if the file exits it will be replaced
	if ((err = config.SetTo("/boot/home/config/settings/TakeNotes/config", B_READ_WRITE |  B_CREATE_FILE | B_ERASE_FILE)) != B_OK){
		return err;
	}

	// Initialiaztion
	toWrite.SetTo("");

	// Writing the structure, for each signature...
	countSignatures = fHash -> GetNumSignatures();
	for (int i = 0; i < countSignatures; i++) {

		// Prepare the string
		char* signature = fHash -> GetSignature(i);
		countNotes = fHash -> GetNumNotes(signature);

		// Parse the string
		for (int j = 0; j < countNotes; j++) {
			char* note = fHash -> GetNote (signature, j);
			printf(">>(FILE) nota: %d, path: %s\n", j, note);
			toWrite.Append(note);
			toWrite.Append(":");
			toWrite.Append(signature);
			toWrite.Append(":");
		}
	}

	// Finally, write all
	config.Write (toWrite.String(), toWrite.Length());

	// Unload the file and return
	config.Unset();
	return B_OK;
}

// Function that handles all the messages that arrives to the view
void NoteView :: MessageReceived(BMessage *message){

	// Variables
	char	*argv[1];

	switch (message->what){

		// The user wants more information about us
		case B_ABOUT_REQUESTED:
			_AboutRequested();
		break;

		// The user wants to open takenotes and write a new note
		case OPEN_TAKENOTES:
			_OpenTakeNotes();
		break;

		// The user wants to open a new note
		case OPEN_NEW_NOTE:
			be_roster -> Launch("application/x-vnd.ccc-TakeNotes", 1, argv, NULL);
		break;

		// The user wants to open a note releated to an application that is currently running
		case OPEN_FILE:{

			// Find the note path of the releated application
			argv[0] = (char*)message -> FindString("Note");

			//Open the note
			be_roster -> Launch("application/x-vnd.ccc-TakeNotes", 1, argv, NULL);
		}
		break;

		// Remove the association between a note and an application
		case REMOVE_ASSOCIATION: {

			// Find what has to be removed
			BString n(message->FindString("Note"));
			BString s(message->FindString("Signature"));

			// Removing from the data structure
			fHash -> DeleteNote (s, n);

			// Rewriting the file
			_SaveDB();
		};
		break;

		/*
		* The user wants to close the view, the view receives this
		* message only when installed in the Deskbar
		*/

		case B_QUIT_REQUESTED:
			_Quit();
		break;

		// All the other messages are forwarded to the superclass method
		default:
			BView::MessageReceived(message);
		break;
	}
}

// Function used to load the database of the associations between an application and a note
void NoteView :: _LoadDB(){

	// Variables
	BFile	config;
	off_t	length;
	char	*input;

	BString stringa;
	BString	path;
	BString signature;

	int32	firstComma;
	int32	lastComma;

	// Check if the file exists and if it is readable
	if (fDatabase.SetTo("/boot/home/config/settings/TakeNotes/config", B_READ_ONLY) != B_OK){

	}

	// Obtain the length of the file and sufficent memory is allocated for the file's contents
	fDatabase.GetSize(&length);
	input = (char *)malloc(length);

	// Actually read the database file
	if (input && (fDatabase.Read(input, length) >= B_OK)){
		stringa.SetTo(input);
		free(input);
	} else {
		return;
	}

	// Deallocate the BFile object
	fDatabase.Unset();

	// Clean the string from random error
	stringa.Remove(stringa.FindLast(":")+1,stringa.CountChars());

	// Parser loop
	while (stringa.CountChars() > 0 ){

		// We look for the initial point
		firstComma = stringa.FindFirst(":")+1;

		// Catch the path and remove the :
		stringa.MoveInto(path, 0, firstComma);
		path.RemoveLast(":");

		// Search for the second :
		lastComma = stringa.FindFirst(":")+1;

		// Catch the signature and remove the :
		stringa.MoveInto(signature, 0, lastComma);
		signature.RemoveLast(":");

		// Add the signature and the note to the Hash Table
		fHash->AddNote(signature,path);
	}
}

// Launch TakeNotes
void NoteView :: _OpenTakeNotes(){

	be_roster->Launch("application/x-vnd.ccc-TakeNotes", new BMessage(OPEN_NEW_NOTE));

}

// Quit the application
void NoteView :: _Quit(){

	if (fInDeskbar){

		BDeskbar deskbar;
		deskbar.RemoveItem(kDeskbarItemName);

	} else {
		be_app->PostMessage(B_QUIT_REQUESTED);
    }

}

// About TakeNotes
void NoteView :: _AboutRequested(){

	BAboutWindow* about = new BAboutWindow("TakeNotes", kSignature);

	const char* authors[] = {
		"Ilio Catallo",
		"Stefano Celentano",
		"Eleonora Ciceri",
		"Florian Thaler",
		NULL
	};

	about->AddCopyright(2021, "Ilio Catallo & others");
	about->AddAuthors(authors);
	about->AddText(B_TRANSLATE("Distributed under the terms of the GPLv2 license"));
	about->AddText(B_TRANSLATE("Icons by Meanwhile"));
	about->AddExtraInfo("extra info");
	about->Show();

}

/*
* Setter function, NoteView::Archive is called several times in several different situations
* (replicant creation, note loading, note saving). The default behaviour is to set
* fReplicated = true everytime Archive is called, and then use this function to
* change the value
*/
void NoteView :: SetReplicated(bool flag){

	fReplicated = flag;

}

// Getter function (see the NoteView::SetReplicated explanation)
bool NoteView :: GetReplicated(){

	return fReplicated;

}

// Set the background color of the view
void NoteView :: SetBackgroundColor(rgb_color color){

	// Find the text view and change the color
	if (FindView("NoteText") != NULL){

		FindView("NoteText")->SetViewColor(color);

		// Tell the view to refresh itself
		FindView("NoteText")->Invalidate();
	}
}

extern "C" _EXPORT BView *instantiate_deskbar_item(void){

	return new NoteView(BRect(0, 0, 15, 15), B_FOLLOW_LEFT | B_FOLLOW_TOP,true);
}
