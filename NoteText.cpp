/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 * 
 * Last revision: Ilio Catallo, 14th May 2009
 *
 * Description: TODO
 */
 

#include "NoteText.h"

#include <Message.h>

// Messages 
#define TEXT_CHANGED 'txch'

//Constants
#define TEXT_INSET 10

//Costructor
NoteText :: NoteText(BRect frame, BRect frameText, char *name, BHandler *handler)
		 : BTextView(frame, name, frameText, B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW){
		 
		 fMessenger = new BMessenger(handler);
		 fReplicated = false;
		 
		 SetViewColor(254, 254, 92, 255);
		 MakeResizable(true);
}


NoteText :: NoteText (BMessage *message)
		 : BTextView (message){
		 
		 fReplicated = true;
		 fBitmap = new BBitmap(message);
		 
}

NoteText :: ~NoteText(){

	delete fBitmap;

}


void NoteText :: Draw(BRect updateRect){
	
	BTextView :: Draw(updateRect);
	
	if (fBitmap){
			
		 BPoint coordinates(PointAt(TextLength()));	
		 coordinates.x = 0;
		 coordinates.y += LineHeight();
		 DrawBitmap(fBitmap,coordinates);
	
	}
	
	

}

status_t NoteText :: Archive (BMessage *msg, bool deep) const{

	BTextView :: Archive(msg,deep);
	
	msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
	msg->AddString("class","NoteText");
	
	if (fBitmap){
	
		fBitmap->Lock();
		fBitmap->Archive(msg);
		fBitmap->Unlock();
	}
	
	return B_OK;

}

BArchivable* NoteText :: Instantiate(BMessage *msg){

	return new NoteText(msg);
	
}

void NoteText :: MessageReceived(BMessage *message){

	switch (message->what){
	
		case B_SIMPLE_DATA:{
		
			//Variables
			entry_ref	ref;
				
			message->FindRef("refs",&ref);
			BEntry entry(&ref);
			BPath path(&entry);
			
			delete fBitmap;
			fBitmap = BTranslationUtils::GetBitmap(path.Path());
			
			if (fBitmap != NULL){
			
				BRect rect = fBitmap->Bounds();
				if (!fReplicated)
					Window()->ResizeTo(rect.right,rect.bottom);
				
				ResizeTo(rect.right, rect.bottom);
				Invalidate();
				//MakeEditable(false);
				
			}
		}
		break;
		
		default:
			BTextView :: MessageReceived(message);
		break;
	}
}
	

void NoteText :: KeyDown(const char* bytes, int32 numBytes){
		
		BTextView :: KeyDown(bytes, numBytes);
		Invalidate();
		//Window()->UpdateIfNeeded();
		
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
