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
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Clipboard.h>

// Messages 
#define TEXT_CHANGED 'txch'

// Constants
#define TEXT_INSET 10

// Costructor
NoteText :: NoteText(BRect frame, BRect frameText, char *name, BHandler *handler)
		 : BTextView(frame, name, frameText, B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE){
		 
	fMessenger = new BMessenger(handler);
	fReplicated = false;
		 
	SetViewColor(254, 254, 92, 255);
		 
}


NoteText :: NoteText (BMessage *message)
		 : BTextView (message){
		 
	fMessenger = NULL;
	fReplicated = true;
		 
}

NoteText :: ~NoteText(){


}



status_t NoteText :: Archive (BMessage *msg, bool deep) const{

	BTextView :: Archive(msg,deep);
	
	msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
	msg->AddString("class","NoteText");
	

	return B_OK;

}

BArchivable *NoteText :: Instantiate(BMessage *msg){

	return new NoteText(msg);
	
}

void NoteText :: MessageReceived(BMessage *message){

	switch (message->what){

		// Edit messages
		case B_CUT:
			this -> Cut(be_clipboard);		
		break;
		
		case B_COPY:
			this -> Copy(be_clipboard);
		break;
		
		case B_PASTE:
			this -> Paste(be_clipboard);
		break;
		
		default:
			BTextView :: MessageReceived(message);
		break;
	}
}
	

	
void NoteText :: FrameResized (float width, float height){
			  
	// Variables
	BRect frameText;
			  
	frameText = Bounds();
	frameText.InsetBy(TEXT_INSET,TEXT_INSET);
	SetTextRect(frameText);
			  
	BTextView :: FrameResized(width,height);
			  		
}
	
void NoteText :: InsertText(const char *text, int32 length, int32 offset, const text_run_array *runs) {
	
	// Variables
	BMessage *message;

	if (fReplicated == false){
    	
    	message = new BMessage (TEXT_CHANGED);
		fMessenger->SendMessage(message);	
	
	}
	BTextView::InsertText(text,length,offset,runs);

}

void NoteText :: MouseDown(BPoint point){

	// Variables
	uint32 		fMouseButtons;
	BMessage 	*currentMessage;
	
	BPopUpMenu 	*popupmenu;
	
	BMenuItem  	*item1;
	BMenuItem 	*item2;
	BMenuItem 	*item3;
	
	BMenuItem   *selected;
	
	// Obtain the current message from window (B_MOUSE_DOWN)
	currentMessage = Window()->CurrentMessage();
	
	
	if(currentMessage) {
	
		// Extract which mouse button information from current message
		currentMessage -> FindInt32("buttons", (int32*)&fMouseButtons);
		
		// If mouse button is the secondary one I show the pop up menu
		if(fMouseButtons == B_SECONDARY_MOUSE_BUTTON) {
			
			// Instantiate pop up menu
			popupmenu = new BPopUpMenu("popupmenu");
	
			// Populate the pop up menu with menu items
			popupmenu -> AddItem(item1 = new BMenuItem("Cut", new BMessage(B_CUT)));
			item1->SetTarget(this);
			popupmenu -> AddItem(item2 = new BMenuItem("Copy", new BMessage(B_COPY)));
			item2->SetTarget(this);
			popupmenu -> AddItem(item3 = new BMenuItem("Paste", new BMessage(B_PASTE)));
			item3->SetTarget(this);
			
			// Convert mouse position to screen coordinates
			ConvertToScreen(&point);

			// Show the pop up menu and get the selected item
			selected = popupmenu -> Go(point, true, true, true);
			
		}	
	
	}
	
	// Go on with MouseDown
	BTextView::MouseDown(point);

}

void NoteText :: SetReplicated(bool flag){

		printf("(NOTE_TEXT) Replicato falso\n");
		fReplicated = flag;
}

bool NoteText :: GetReplicated(){
	
		return fReplicated;

}

void NoteText :: SetHandler(BHandler *handler){

		if (fMessenger)
			delete fMessenger;
		
		fMessenger = new BMessenger(handler);
		

}
