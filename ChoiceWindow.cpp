/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Ilio Catallo, 7th June 2009
 *
 * Description: TODO
 */

#include "ChoiceWindow.h"
#include <Alert.h>
#include <Message.h>
#include <stdio.h>

#define RADIO_CHECKED 'rdck'
#define BUTTON_OK		'btok'
#define BUTTON_UNDO		'btun'

// Constructor
// It is created with the dimensions of BRect
ChoiceWindow :: ChoiceWindow (BRect frame, BHandler *handler)
			: BWindow (frame, "Choose an application", B_TITLED_WINDOW,B_NOT_RESIZABLE) {
	
	//Inizialization
	fCurrentCheckedMessage = new BMessage(RADIO_CHECKED);
	
	//Create a Messenger and set its target as NoteWindow
	fMessenger = new BMessenger(handler);
	
	//Create the view that shows the list of the current opened applications
	fChoiceView = new ChoiceView (Bounds(), "ChoiceView",handler); // prima era frame
	
	//Add the view as a child
	AddChild(fChoiceView);	   	   
	   	   
	//Show the window
	Show();
}

// Function that is activated when I close the window
bool ChoiceWindow :: QuitRequested() {
	Quit();	
	return (true);
}


void ChoiceWindow :: MessageReceived(BMessage* message) {
	
	printf("sono nella window\n");

	
	switch(message -> what) {
	
		case RADIO_CHECKED:{
		
			fCurrentCheckedMessage = message;
			
		}
		break;
		
		case BUTTON_OK:{
			fCurrentCheckedMessage->PrintToStream();
			//fMessenger->SendMessage(fCurrentCheckedMessage);
			Quit();
		
		}
		break;
		
		case BUTTON_UNDO:{
		
			BAlert* alert = new BAlert("", "Do you really want to close the window ?", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->SetShortcut(0, B_ESCAPE);

			if (alert->Go() == 0) {
				//Discard all the changes
				Quit();
			}
	
		}
	
		default:
			BWindow::MessageReceived(message);
			break;
	}
}