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
 * Last revision: Ilio Catallo, 23th April 2009
 *
 * Description: TODO
 */


#include "NoteView.h"

#include <Dragger.h>
#include <Alert.h>

#include <stdio.h>
#include <stdlib.h>

// Messages
#define TEXT_CHANGED 	'txch'
#define FONT_BOLD 		'fntb'

// Constructor
NoteView :: NoteView(BRect frame,BRect frameText, char *name, BHandler *handler )
	   	 : BTextView(frame, name, frameText, B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW){
	   	   	   	   
	   	   fMessenger = new BMessenger(handler);
		   Init();	   	   
}

NoteView :: NoteView (BMessage *msg)
		   : BTextView(msg){
			
			Init();

}

// Destructor
NoteView :: ~NoteView(){}

// Initialization
void NoteView :: Init(){

		// Variable
		BDragger *dragger;	
		
		dragger = new BDragger(BRect(0,270,7,277),this,B_FOLLOW_ALL);

		SetViewColor(254,254,92,255);
		AddChild(dragger);
}
		
		
status_t NoteView :: Archive (BMessage *msg,bool deep) const{
		
		//Variable
		status_t result;
		
		
		result = BTextView ::Archive(msg,deep);
		
		if (result == B_OK){
			result = msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
			
			if (result == B_OK) 
				result = msg->AddString("class","NoteView");
		
		}
		
		return result;

}

BArchivable* NoteView :: Instantiate(BMessage *msg){

		if (!validate_instantiation(msg,"NoteView")) 
			return NULL;
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

	SetViewColor(color);
	Invalidate();
		
}


void NoteView :: InsertText(const char* text, int32 length, int32 offset, const text_run_array *runs) {
	// Variables
	BMessage *message;

    message = new BMessage (TEXT_CHANGED);
	fMessenger->SendMessage(message);	
	BTextView::InsertText(text,length,offset,runs);

}