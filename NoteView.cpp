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
 * Last revision: Eleonora Ciceri, 30th May 2009
 *
 * Description: TODO
 */


#include "NoteView.h"

#include <Alert.h>
#include <Dragger.h>
#include <File.h>
#include <NodeInfo.h>
#include <FilePanel.h>

#include <stdio.h>
#include <stdlib.h>

// Messages
#define FONT_BOLD 		'fntb'
#define CHOOSE_APPL		'cspp'

// Constructor
NoteView :: NoteView(BRect frame, BHandler *handler)
	   	 : BView(frame, "TakeNotes", B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE){
	   	   	   	   
	   	// Variable
		BDragger *dragger;	
		
		fNoteText = NULL;
		
		dragger = new BDragger(BRect(0,0,7,7),this,B_FOLLOW_NONE);
		AddChild(dragger);
		
		SetViewColor(254,254,92,255); 
		
		fMessenger = new BMessenger(handler); 
		   
}

NoteView :: NoteView (BMessage *msg)
		   : BView(msg){
			
			fReplicated = true;

}

// Destructor
NoteView :: ~NoteView(){}
		
		
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
			message = new BMessage(CHOOSE_APPL);
			fMessenger -> SendMessage(message);
			printf("QUI è lanciato\n");
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

		alert = new BAlert("TakeNotes","A program to take notes","OK");
		alert->SetShortcut(0,B_ESCAPE);
		alert->Go();

}


void NoteView :: MessageReceived(BMessage *message){

		message->PrintToStream();

		switch(message->what){
		
			case B_ABOUT_REQUESTED:
				AboutRequested();
				break;
				
			default:
				BView::MessageReceived(message);
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


