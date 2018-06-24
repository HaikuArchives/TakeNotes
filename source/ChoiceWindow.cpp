/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 *
 * Last revision: Eleonora Ciceri, 30th June 2009
 *
 * Description: ChoiceWindow; it allow you to select one application and associate it to a saved note
 */

// Our Libraries
#include "ChoiceWindow.h"

// Libraries
#include <Alert.h>
#include <Message.h>
#include <stdio.h>

// Messages
#define RADIO_CHECKED 	'rdck'
#define BUTTON_OK		'btok'
#define BUTTON_UNDO		'btun'
#define CHOICE_CLOSE	'_chc'

/*
* Constructor
* It is created with the dimensions of BRect
*/
ChoiceWindow :: ChoiceWindow (BRect frame, BHandler *handler)
			: BWindow (frame, "Choose an application", B_TITLED_WINDOW,B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS) {

	// Inizialization
	fCurrentCheckedMessage = NULL;

	// Create a Messenger and set its target as NoteWindow
	fMessenger = new BMessenger(handler);

	// Create the view that shows the list of the current opened applications
	fChoiceView = new ChoiceView (Bounds(), "ChoiceView", handler); // prima era frame

	// Set the background color
	fChoiceView->SetViewColor(216, 216, 216, 0);

	// Add the view as a child
	AddChild(fChoiceView);

	// Show the window
	Show();
}

// Destructor
ChoiceWindow :: ~ChoiceWindow(){
	delete fCurrentCheckedMessage;
}

// Manages the message received
void ChoiceWindow :: MessageReceived(BMessage* message) {
	// Variables
	BAlert *alert;

	switch(message -> what) {

		// If a new Radio Button is checked we change the message which stores the current selection
		case RADIO_CHECKED:{

			if (fCurrentCheckedMessage != message){

				delete fCurrentCheckedMessage;
				fCurrentCheckedMessage = new BMessage(*message);

			}
		}
		break;

		// Send the user choice to NoteWindow and quit
		case BUTTON_OK:{

			fMessenger->SendMessage(fCurrentCheckedMessage);
			Quit();

		}
		break;

		// It capture the UNDO request
		case BUTTON_UNDO:{

			// Ask before quit
			alert = new BAlert("", "Do you really want to close the window ?", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->SetShortcut(0, B_ESCAPE);

			if (alert->Go() == 0) {
				// Discard all the changes
				Quit();
			}

		}
		break;

		default:
			BWindow::MessageReceived(message);
			break;
	}
}

void ChoiceWindow :: Quit(){
	//Variables
	BMessage *message;

	// Inform NoteWindow that this window is going to be closed
	message = new BMessage (CHOICE_CLOSE);
	fMessenger->SendMessage(message);

	// Execute the real code
	BWindow :: Quit();
}

// Function that is activated when I close the window
bool ChoiceWindow :: QuitRequested() {
	BMessage *message;

	// Message that tells that the window is going to close
	message = new BMessage (CHOICE_CLOSE);
	fMessenger->SendMessage(message);

	Quit();
	return (true);
}
