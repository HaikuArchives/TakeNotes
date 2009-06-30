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
 * Description: NoteText is the view that accept text from the user
 */
 
//Our Libraries
#include "NoteText.h"

// Libraries
#include <Clipboard.h>
#include <Message.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Window.h>

// Messages 
#define TEXT_CHANGED 'txch'

// Constants
#define TEXT_INSET 10

// Costructor
NoteText :: NoteText(BRect frame, BRect frameText, char *name, BHandler *handler)
		 : BTextView(frame, name, frameText, B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE){
	
	// Initialization 	 
	fMessenger = new BMessenger(handler);
	fReplicated = false;
		 
	// Background color 
	SetViewColor(254, 254, 92, 255);
		 
}

/*
* Costructor by message
* We can restore a NoteText starting from a BMessage that contains its structure
*/ 
NoteText :: NoteText (BMessage *message)
		 : BTextView (message){

	// Initialization		 
	fMessenger = NULL;
	fReplicated = true;
		 
}

// Destructor (actually empty, no dynamic object to destroy here)
NoteText :: ~NoteText(){}

// Function that is called when the NoteText is archived as a part of a BMessage
status_t NoteText :: Archive (BMessage *msg, bool deep) const{

	// Actually execute the real code
	BTextView :: Archive(msg,deep);
	
	/*
	* Flags the message with the information regarding the application and the class name
	* This informations are required and they will be used by the costructor by message 
	* to rehydrate the replicant
	*/
	msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
	msg->AddString("class","NoteText");
	
	return B_OK;

}

/* 
* This function is called when we restore the note from a BMessage
* so it's called 3 times: 	- when we rehydrated a replicant
*							- when we restore a saved note
*							- when we install the application in the Deskbar
*/
BArchivable *NoteText :: Instantiate(BMessage *msg){

	return new NoteText(msg);
	
}

// Function that handles all the messages that arrives to the view
void NoteText :: MessageReceived(BMessage *message){

	switch (message->what){

		// Cut request
		case B_CUT:
			this -> Cut(be_clipboard);		
		break;
		
		// Copy Request
		case B_COPY:
			this -> Copy(be_clipboard);
		break;
		
		// Paste Request
		case B_PASTE:
			this -> Paste(be_clipboard);
		break;
		
		// All the other messages are forwarded to the superclass method
		default:
			BTextView :: MessageReceived(message);
		break;
	}
}
	
/*
* Hook function-override, we the view is resized we have to change
* the current writable area to suite the new size
*/	
void NoteText :: FrameResized (float width, float height){
			  
	// Variables
	BRect frameText;
			  
	// Obtain the current size of the view and adapt the writable area
	frameText = Bounds();
	frameText.InsetBy(TEXT_INSET,TEXT_INSET);
	SetTextRect(frameText);
	
	// All the rest is handled by the superclass method		  
	BTextView :: FrameResized(width,height);
			  		
}

/*
* Hook function-override, when the user insert a character we should inform NoteWindow 
* so it can activate the UNDO voice in the Edit menu
*/
void NoteText :: InsertText(const char *text, int32 length, int32 offset, const text_run_array *runs) {
	
	// Variables
	BMessage *message;

	// If the view lives as a replicant it mustn't try to inform anyone
	if (fReplicated == false){
    	
    	message = new BMessage (TEXT_CHANGED);
		fMessenger->SendMessage(message);	
	
	}
	
	// All the rest is handled by the superclass method
	BTextView::InsertText(text,length,offset,runs);

}

/*
* Hook function-override, this function shows the contextual menu only
* if the user click with the right button 
*/
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
			popupmenu -> AddItem(item2 = new BMenuItem("Copy", new BMessage(B_COPY)));
			popupmenu -> AddItem(item3 = new BMenuItem("Paste", new BMessage(B_PASTE)));
			
			item1->SetTarget(this);			
			item2->SetTarget(this);
			item3->SetTarget(this);
			
			// Convert mouse position to screen coordinates
			ConvertToScreen(&point);

			// Show the pop up menu and get the selected item
			selected = popupmenu -> Go(point, true, true, true);
			
		}	
	
	}
	
	// All the rest is handled by the superclassh method
	BTextView::MouseDown(point);

}

/*
* Setter function, NoteText::Archive is called several times in several different situations
* (replicant creation, note loading, note saving). The default behaviour is to set
* fReplicated = true everytime Archive is called, and then use this function to 
* change the value
*/
void NoteText :: SetReplicated(bool flag){

		fReplicated = flag;
}

// Getter function (see the NoteText::SetReplicated explanation)
bool NoteText :: GetReplicated(){
	
		return fReplicated;

}

/*
* function that receives a BHandler and create a BMessenger object starting from it.
* NoteWindow is passed to this function to allows comunications between NoteWindow and NoteText.
* Usually a NoteWindow reference is passed when NoteText is created but when we restore a saved
* note NoteWindow has to restore the reference.
*/
void NoteText :: SetHandler(BHandler *handler){

		if (fMessenger)
			delete fMessenger;
		
		fMessenger = new BMessenger(handler);
		

}
