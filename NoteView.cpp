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
 * Last revision: Ilio Catallo, 13th June 2009
 *
 * Description: TODO
 */


#include "NoteApplication.h"
#include "NoteView.h"

#include <Application.h>
#include <Alert.h>
#include <Deskbar.h>
#include <Dragger.h>
#include <File.h>
#include <FilePanel.h>
#include <MenuItem.h>
#include <NodeInfo.h>
#include <PopUpMenu.h>
#include <String.h>
#include <Roster.h>

#include <stdio.h>
#include <stdlib.h>

// Messages
#define FONT_BOLD 		'fntb'
#define CHOOSE_APPL		'cspp'
#define	OPEN_TAKENOTES	'onwp'
#define	OPEN_FILE		'onfl'

// Constructor
NoteView :: NoteView(BRect frame, int32 resizingMode, bool inDeskbar, BHandler *handler)
	   	 : BView(frame, "TakeNotes", resizingMode, B_FRAME_EVENTS | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	   		fInDeskbar(inDeskbar){
	   	   	   	   
	   	// Variable
		BDragger *dragger;	
		
		//Initiliazation
		fNoteText = NULL;
		if (handler) fMessenger = new BMessenger(handler); 
		fHash = new AppHashTable();
		
		//Graphical Stuff
		SetViewColor(254,254,92,255); 
		if (!inDeskbar){
		
			//We don't have to add the dragger if the view is in the deskbar
			dragger = new BDragger(BRect(0,0,7,7),this,B_FOLLOW_NONE);
			AddChild(dragger);
		}
		
		   
}

NoteView :: NoteView (BMessage *msg, BHandler *handler)
		   : BView(msg){
			
			//Variables
			app_info	info;
			
			//Initialization
			fReplicated = true;
			if (handler) fMessenger = new BMessenger(handler);
			
			//Check if we are in the deskbar
			if (be_app->GetAppInfo(&info) == B_OK && !strcasecmp(info.signature, "application/x-vnd.Be-TSKB")){
				fInDeskbar = true;
				//_LoadDB();

			} else {
	
				printf("%s\n",info.signature);
			
			}
			
}

// Destructor
NoteView :: ~NoteView(){}
		

void NoteView :: MouseDown(BPoint point){


	// Load the database of notes
	fHash = new AppHashTable();
	_LoadDB();

	BPopUpMenu* menu = new BPopUpMenu(B_EMPTY_STRING, false, false);
	menu->SetAsyncAutoDestruct(true);
	menu->SetFont(be_plain_font);

	menu->AddItem(new BMenuItem("Open TakeNotes" B_UTF8_ELLIPSIS,new BMessage(OPEN_TAKENOTES)));
	menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED)));
	menu->AddSeparatorItem();

				
	// Initialization
    BList *aList = new BList;
    app_info *appInfo = new app_info();
    int count;
    entry_ref	ref;
    BString		name;

   	// Obtaining the applications that are running
   	be_roster->GetAppList(aList); 

	// We look for the current instances that are running
	for (count=0;count< aList->CountItems();count++){
   	    team_id who = (team_id)aList->ItemAt(count);
   	  	be_roster->GetRunningAppInfo(who,appInfo);
  		 	            	
         	
       	// Show the name of the running program istead of its signature
        ref = appInfo->ref;
   		name.SetTo(ref.name);
   		 	            	
   		//not show the system process like the *_server program or the Deskbar / Tracker
   		if (name.Compare("registrar") != 0 && name.Compare("Tracker") != 0 && name.Compare("Deskbar") != 0 &&
   		 	name.Compare("net_server") != 0 && name.Compare("syslog_daemon") !=0 && name.Compare("input_server") != 0 &&
   		 	name.Compare("midi_server") !=0 && name.Compare("print_server") !=0 && name.Compare("media_server") != 0 &&
   		 	name.Compare("media_addon_server") != 0 && name.Compare("afp_server") != 0 && name.Compare("debug_server") != 0) {
   		 	
   		 	// Obtain the signature number
   			int countSignatures = fHash -> GetNumSignatures();
   			
   			//For each signature in the list we check if it is the hash table and in case we add it to the menu
   			for (int i = 0; i < countSignatures; i++) {
   				char *sig = fHash -> GetSignature(i);
   				if (strcmp (appInfo -> signature, sig) == 0) {
   					BMenu *subMenu = new BMenu (ref.name);
   					
   					// Make the list of the notes releated to that application
   					int countNotes = fHash -> GetNumNotes(sig);
   					for (int j = 0; j < countNotes; j++) {
   						char *note = fHash -> GetNote(sig, j);
   						BMessage *mess = new BMessage(OPEN_FILE);
   						mess -> AddString("Note", note);
   						mess -> AddPointer("team",(void *)&who); // We pass also the team_id in order to focus the application
   						subMenu -> AddItem (new BMenuItem(note, mess));
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
		
		
		

		
status_t NoteView :: Archive (BMessage *msg,bool deep) const{
				
		//Variables
		BFile 		file;
		BNodeInfo 	nodeinfo;
		BString		string;
		entry_ref	ref;
		status_t	err;
		const char*	name;
		BDirectory	dir;
		BMessage	*message;
		
		BView ::Archive(msg,deep);
		
		msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
		msg->AddString("class","NoteView");
		
		if (msg->FindRef("directory",&ref) == B_OK
			&& msg -> FindString("name", &name) == B_OK) {
			dir.SetTo(&ref);
			if (err = dir.InitCheck() != B_OK)
				return err;
			file.SetTo(&dir, name, B_READ_WRITE | B_CREATE_FILE);
		}
		else {
			file.SetTo("/boot/home/Desktop/nota.tkn", B_CREATE_FILE | B_WRITE_ONLY);
					
			// Message used to say that a choice of an application is required
			/*message = new BMessage(CHOOSE_APPL);
			fMessenger -> SendMessage(message);
			printf("QUI è lanciato\n");*/
		}
		
		/*if (file.SetTo("/boot/home/Desktop/nota.tkn", B_CREATE_FILE | B_WRITE_ONLY) != B_OK)
				printf("errore\n");*/
		
		msg->Flatten(&file);
		msg->PrintToStream();
		
		nodeinfo.SetTo(&file);
		nodeinfo.SetType("application/takenotes");
		
		file.Unset();
		
		return B_OK;

}

BArchivable* NoteView :: Instantiate(BMessage *msg){

		return new NoteView(msg);
		
}

void NoteView :: AboutRequested(){

		// Variable
		BAlert *alert;

		alert = new BAlert("About TakeNotes", 
				"Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved", 
				"OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
		alert->SetShortcut(0,B_ESCAPE);
		alert->Go();

}


void NoteView :: MessageReceived(BMessage *message){

		message->PrintToStream();

		switch(message->what){
		
			case B_ABOUT_REQUESTED:
				AboutRequested();
			break;
				
			case OPEN_TAKENOTES:
				_OpenTakeNotes();
			break;
				
			case OPEN_FILE:{
				
				//Variables
				char	*argv[1];
				void	*who;

				//Find the note path and the team_id of the releated application	
				argv[0] = (char*) message -> FindString("Note");
				message -> FindPointer("team", &who);
					
				//Open the note and make focus on the releated application
				be_roster -> Launch("application/x-vnd.ccc-TakeNotes", 1, argv, NULL);
				be_roster -> ActivateApp(*static_cast<team_id*>(who));
			
			}
			break;
			
			case B_QUIT_REQUESTED:
				_Quit();
			break;
			
				
			default:
				BView::MessageReceived(message);
			break;
			
		}
}

void NoteView :: _LoadDB(){
	//Variables
	BFile	config;
	off_t	length;
	char*	input;

	BString stringa;
	BString	path;
	BString signature;
	
	//char*	sgn;
	//char* 	pht;
	
	int32	firstComma;
	int32	lastComma;
			
		// Check if the file exists and if it is readable
		if (fDatabase.SetTo("/boot/home/config/settings/TakeNotes/config", B_READ_ONLY) != B_OK){

		}
		
		//Obtain the length of the file and sufficent memory is allocated for the file's contents
		fDatabase.GetSize(&length);
		input = (char *)malloc(length);
	
		// Actually read the database file
		if (input && (fDatabase.Read(input, length) >= B_OK)){
		
			stringa.SetTo(input);
			free(input);
		
		
		} else {
		
			printf("errore di lettura file\n");
			return;
			
		}
		
		//Clean the string from random error
		printf("prima della pulizia: %s\n\n",stringa.String());
		stringa.Remove(stringa.FindLast(":")+1,stringa.CountChars());
		printf("dopo la pulizia: %s\n\n\n",stringa.String());
	
	
		
		while (stringa.CountChars() > 0 ){
	
			//Trovo i due punti iniziali
			firstComma = stringa.FindFirst(":")+1;
		
			// Estrapola il path ed elimina i due punti
			stringa.MoveInto(path, 0, firstComma);
			path.RemoveLast(":");
		
			//Trova i secondi due punti
			lastComma = stringa.FindFirst(":")+1;
		
			// Estrapola la signature ed elimina i due punti
			stringa.MoveInto(signature, 0, lastComma);
			signature.RemoveLast(":");
		
			printf("path: %s\n",path.String());
			printf("signature: %s\n",signature.String());
			printf("TODO: %s\n\n",stringa.String());
	
			
			fHash->AddNote(signature,path);
		}

}




void NoteView :: _OpenTakeNotes(){

	be_roster->Launch("application/x-vnd.ccc-TakeNotes");
	
}

void NoteView :: _Quit(){

	if (fInDeskbar){
	
		BDeskbar deskbar;
		deskbar.RemoveItem(kDeskbarItemName);
	
	} else {
	
		be_app->PostMessage(B_QUIT_REQUESTED);
    }

}

void NoteView :: SetBackgroundColor(rgb_color color){

	if (fNoteText == NULL)
		fNoteText = (NoteText *)FindView("NoteText");

	SetViewColor(color);
	fNoteText -> SetViewColor(color);
		
	Invalidate();
	fNoteText -> Invalidate();	
}

extern "C" _EXPORT BView *instantiate_deskbar_item(void){

		return new NoteView(BRect(0, 0, 15, 15), B_FOLLOW_LEFT | B_FOLLOW_TOP,true);
}
