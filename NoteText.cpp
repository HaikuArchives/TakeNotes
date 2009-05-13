/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 * 
 * Last revision: Ilio Catallo, 13th May 2009
 *
 * Description: TODO
 */
 

#include "NoteText.h"

#include <Message.h>
 
// Messages 
#define TEXT_CHANGED 	'txch'

//Costructor
NoteText :: NoteText(BRect frame, BRect frameText, char *name, BHandler *handler)
		 : BTextView(frame, name, frameText, B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW){
		 
		 fMessenger = new BMessenger(handler);
		 fReplicated = false;
		 
		 SetViewColor(254, 254, 92, 255);
}


NoteText :: NoteText (BMessage *message)
		 : BTextView (message){
		 
		 fReplicated = true;
		 
}

status_t NoteText :: Archive (BMessage *msg, bool deep) const{

	BTextView :: Archive(msg,deep);
	
	//msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
	msg->AddString("class","NoteText");
	
	return B_OK;

}

BArchivable* NoteText :: Instantiate(BMessage *msg){

	return new NoteText(msg);
	
}

void NoteText :: InsertText(const char* text, int32 length, int32 offset, const text_run_array *runs) {
	// Variables
	BMessage *message;

	if (fReplicated == false){
    	
    	message = new BMessage (TEXT_CHANGED);
		fMessenger->SendMessage(message);	
	
	}
	
	BTextView::InsertText(text,length,offset,runs);

}
