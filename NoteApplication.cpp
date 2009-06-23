/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Ilio catallo, 21th June 2009
 *
 * Description: TODO
 */
 
#include "NoteApplication.h"

#include <Alert.h>
#include <AppFileInfo.h>
#include <Autolock.h>
#include <Bitmap.h>
#include <Deskbar.h>
#include <Entry.h>
#include <File.h>
#include <IconUtils.h>
#include <Mime.h>
#include <Node.h>
#include <Path.h>
#include <Resources.h>
#include <Roster.h>

#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

#define COLOR_CHANGED 	'ccrq'
#define FONT_BOLD 		'fntb'

NoteApplication *note_app;


//Usuful constants (we don't want to repeat these hard to remember strings everywhere!)
const char*	kSignature = "application/x-vnd.ccc-TakeNotes";
const char*	kDeskbarSignature = "application/x-vnd.Be-TSKB";
const char*	kDeskbarItemName = "TakeNotes";


// This function fills the image_info image struct with the app image of itself
// and eventually return B_OK
status_t our_image(image_info& image){
	
	int32 cookie = 0;
	
	while (get_next_image_info(B_CURRENT_TEAM, &cookie, &image) == B_OK) {
	
		if ((char *)our_image >= (char *)image.text
			&& (char *)our_image <= (char *)image.text + image.text_size)
			return B_OK;
	}

	return B_ERROR;
}




NoteApplication :: NoteApplication()
			    : BApplication("application/x-vnd.ccc-TakeNotes"){	
	
		//Check (and install) the MIME type
		CheckMime();
		
		// Creation of the directory with the association between notes
		// and applications
		DIR *dir;
		dir = opendir("/boot/home/config/settings/TakeNotes");
		if (dir == NULL)
			mkdir ("/boot/home/config/settings/TakeNotes", O_CREAT);
	
		//private data members initialization
		fWindowCount = 0;
		fWindowCountUntitled = 0;
		note_app = this;

}	

void NoteApplication :: ReadyToRun(){

		//Variables
		BDeskbar	deskbar;
		
		//Check if the replicant isn't already installed in the Deskbar, avoid to ask if we already opened a note
		if (!deskbar.HasItem(kDeskbarItemName) && fWindowCount == 0){
		
			BAlert* alert = new BAlert("", "Do you want TakeNotes to live in the Deskbar?", "Don't", "Install", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->SetShortcut(0, B_ESCAPE);

			if (alert->Go() == 1) {
				_InstallReplicantInDeskbar();
				return;
			}
			
		}

		if (fWindowCount > 0)
			return;
		else
			OpenNote();

}


void NoteApplication :: CheckMime(){

		//Variables
		BMimeType 	takenotes("application/takenotes");
		image_info 	info;
		image_info	appInfo;
		entry_ref	ref;
		

		//Set the icon for the file labeled as application/takenotes
		
		//Variables
		size_t		size;
const	void		*data = NULL;
		BFile		file;
		BResources	resource;
		
		
		app_info 		appInfo2;
		BFile			file2;
		BAppFileInfo	appFileInfo;
		
		//Create a BAppFileInfo object linked to the application itself
		be_app->GetAppInfo(&appInfo2);
		file2.SetTo(&appInfo2.ref, B_READ_WRITE);
		appFileInfo.SetTo(&file2);
		
		//Grab the file icon stored in the resources
		if (our_image(appInfo) != B_OK)
			printf("errore nell'our_image\n");

		file.SetTo(appInfo.name, B_READ_ONLY);
		if (resource.SetTo(&file) != B_OK){
			
			printf("errore nella resource\n");
			
		}
			
		//Load the icon as a blob from the resource	
		data = resource.LoadResource(B_VECTOR_ICON_TYPE,2,&size);
		if (data != NULL){
			
				//Set the icon for the mimetype
				appFileInfo.SetIconForType(takenotes.Type(), (uint8 *)data, size);
						
			
		} else {
			
			printf("i dati erano vuoti\n");
			
		}
		
		
		
		//Check if the mimetype is installed or not
		if (takenotes.InitCheck() == B_OK){
		
			//Variables
			BMessage *msg = new BMessage();
		
		
			//Check if the mimetype is already installed in the DB
			if (takenotes.IsInstalled()){
			
					printf("già installato\n");
					
					delete msg;
					return;
			}	
			
			
			//First of all we install the TakeNotes MIME type
			if (takenotes.Install() != B_OK)
					printf("non installato, errore\n");
			
			
			//We define the extensions for the mime type
			msg->AddString("extensions","tkn");
			
			if (takenotes.SetFileExtensions(msg) != B_OK){
				printf("errore nel settare l'estensione");
				exit(-1);
			}
			
			//Set the preferred application	
			if (takenotes.SetPreferredApp("application/x-vnd.ccc-TakeNotes") != B_OK){
				printf("errore nel settare l'app preferred\n");
				exit(-1);
			}
			
			//Set the Hint application, using the our_image function we are able to find
			//the current path of the application and pass it to the FileType DB
			if (our_image(info) == B_OK && get_ref_for_path(info.name, &ref) == B_OK){
			
					BPath prova(&ref);
					printf("path %s\n",prova.Path());
					
					if (takenotes.SetAppHint(&ref) != B_OK)
						printf("errore nell'app hint\n");
			
			}
			
			//Add some extra attributes in order to manage custom tags
			BMessage attr;
			
			
			attr.AddString("attr:name","TAKENOTES:refapp");
			attr.AddString("attr:name","TAKENOTES:type");
			attr.AddString("attr:name","TAKENOTES:tagone");
			attr.AddString("attr:name","TAKENOTES:tagtwo");
			attr.AddString("attr:name","TAKENOTES:tagthree");
			
			attr.AddString("attr:public_name","refapp");
			attr.AddString("attr:public_name","type");
			attr.AddString("attr:public_name","tagone");
			attr.AddString("attr:public_name","tagtwo");
			attr.AddString("attr:public_name","tagthree");
		
			attr.AddInt32("attr:type",B_STRING_TYPE);
			attr.AddInt32("attr:type",B_STRING_TYPE);
 			attr.AddInt32("attr:type",B_STRING_TYPE);
 			attr.AddInt32("attr:type",B_STRING_TYPE);
			attr.AddInt32("attr:type",B_STRING_TYPE);
		
			attr.AddInt32("attr:width",60);
			attr.AddInt32("attr:width",60);
			attr.AddInt32("attr:width",60);
			attr.AddInt32("attr:width",60);
			attr.AddInt32("attr:width",60);
			attr.AddInt32("attr:width",60);

			attr.AddBool("attr:viewable",true);
			attr.AddBool("attr:viewable",true);
			attr.AddBool("attr:viewable",true);
			attr.AddBool("attr:viewable",true);
			attr.AddBool("attr:viewable",true);
			
			attr.AddBool("attr:editable",true);
			attr.AddBool("attr:editable",true);
			attr.AddBool("attr:editable",true);
			attr.AddBool("attr:editable",true);
			attr.AddBool("attr:editable",true);
			
			attr.AddBool("attr:extra",true);
			attr.AddBool("attr:extra",true);
			attr.AddBool("attr:extra",true);
			attr.AddBool("attr:extra",true);
			attr.AddBool("attr:extra",true);
			
			attr.AddInt32("attr:alignment",0);
			attr.AddInt32("attr:alignment",0);
			attr.AddInt32("attr:alignment",0);
			attr.AddInt32("attr:alignment",0);
			attr.AddInt32("attr:alignment",0);
					
			//attr->PrintToStream();
		
			if (takenotes.SetAttrInfo(&attr) != B_OK){
					printf("errore nel settare i metadata\n");
			}
				
			//Add a short description for the MIME type
			if (takenotes.SetShortDescription("TakeNotes") != B_OK)
					printf("errore nel settare la short description\n");
		
			//Gargabe collection
			delete msg;
	
		} else {
		
				printf("errore di init del mimetype");
		}


}

void NoteApplication :: OpenNote(){

		
		new NoteWindow(fWindowCountUntitled++);
		fWindowCount++;

}

void NoteApplication :: OpenNote(entry_ref *ref){


		new NoteWindow(ref);
		fWindowCount++;

}

void NoteApplication :: CloseNote(){

		fWindowCount--;
		if (fWindowCount == 0){
		
			BAutolock lock(this);
			Quit();		
		}

} 


void NoteApplication :: ArgvReceived(int32 argc, char** argv){

		//Variables
		const 	char* 		cwd = "";
				BMessage 	*message = CurrentMessage();
		
		
		//Extract the cwd (current working directory)
		if (message != NULL){
		
			if (message->FindString("cwd",&cwd) != B_OK)
					cwd = "";
		}
		
		//Check if it is an absolute or relative path
		//If the path is a relative one we make it absolute
		//Eventually we open the note(s)
		for (int i=1; i < argc; i++){
		
			//Variables
			BPath path;
			entry_ref ref;
			
			if (argv[i][0] == '/'){
				
				path.SetTo(argv[i]);
			
			} else {
			
				path.SetTo(cwd, argv[i]);
			}
			
			get_ref_for_path(path.Path(), &ref);
			OpenNote(&ref);
			
		}

}

void NoteApplication :: RefsReceived(BMessage *message){

		//Variables
		int32 index = 0;
		entry_ref ref;
		
		while(message->FindRef("refs", index++, &ref) == B_OK){
		
			OpenNote(&ref);
		
		}
}


void NoteApplication :: MessageReceived(BMessage *message){


		switch(message->what){
		
		
			case B_SILENT_RELAUNCH:
				OpenNote();
				break;
			
			default:
				BApplication::MessageReceived(message);
				break;
		
		}

}

void NoteApplication :: _InstallReplicantInDeskbar(){

		//Variables
		image_info info;
		entry_ref	ref;
		
		//If it succeded in finding itself
		if (our_image(info) == B_OK && get_ref_for_path(info.name, &ref) == B_OK){
		
			printf("tento di installare\n");
			
			//Find itself in the file system
			BPath path(&ref);
			printf("path %s\n",path.Path());
			
			//Initialize the deskbar object
			BDeskbar deskbar;
			
			//If the deskbar is running we finally install 
			if (!deskbar.IsRunning())
				printf("la deskbar NON sta andando\n");
			
			if (deskbar.AddItem(&ref) != B_OK)
				printf("errore nell'additem\n");
		
		} else {
		
			printf("errore\n");
		
		}
		
		//Quit the application, an indipendent instance of TakeNotes is now running
		//as a replicant in the Deskbar
		Quit();


}



// Main
int main(){

	NoteApplication myApp;
	
	myApp.Run();
	return(0);
}		

