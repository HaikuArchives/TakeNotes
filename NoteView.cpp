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
 * Last revision: Ilio Catallo, 13th May 2009
 *
 * Description: TODO
 */


#include "NoteView.h"

#include <Dragger.h>
#include <Alert.h>

#include <stdio.h>
#include <stdlib.h>

// Messages
#define FONT_BOLD 		'fntb'

// Constructor
NoteView :: NoteView(BRect frame, char *name)
	   	 : BView(frame, name, B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW){
	   	   	   	   
	   	// Variable
		BDragger *dragger;	
		
		fNoteText = NULL;
		
		dragger = new BDragger(BRect(0,0,7,7),this,B_FOLLOW_NONE);
		AddChild(dragger);
		
		SetViewColor(254,254,92,255);  
		   
}

NoteView :: NoteView (BMessage *msg)
		   : BView(msg){
			
			fReplicated = true;

}

// Destructor
NoteView :: ~NoteView(){}
		
		
status_t NoteView :: Archive (BMessage *msg,bool deep) const{
				
		BView ::Archive(msg,deep);
		
		msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
		msg->AddString("class","NoteView");
		
		msg->PrintToStream();
		
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


