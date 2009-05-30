/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 30th May 2009
 *
 * Description: TODO
 */

#include "ChoiceWindow.h"

#define RADIO_CHECKED 'rdck'

// Constructor
// It is created with the dimensions of BRect
ChoiceWindow :: ChoiceWindow (BRect frame, BHandler *handler)
			: BWindow (frame, "Choose an application", B_TITLED_WINDOW,B_NOT_RESIZABLE) {
	// Variables
	BPoint 				leftTop(20.0, 50.0);
	
	frame.OffsetTo(B_ORIGIN);
	fChoiceView = new ChoiceView (frame, "ChoiceView",handler);
	AddChild(fChoiceView);	   	   
	   	   
	fMessenger = new BMessenger(handler);
	
	Show();
}

// Function that is activated when I close the window
bool ChoiceWindow :: QuitRequested() {
	return (true);
}

void ChoiceWindow :: MessageReceived(BMessage* message) {
	
	switch(message -> what) {
		case RADIO_CHECKED: {
			fMessenger -> SendMessage(message);
		}
		break;
		default:
			break;
	}
}

