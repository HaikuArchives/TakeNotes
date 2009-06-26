/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 23th June 2009
 *
 * Description: TODO
 */

#include "ColorWindow.h"

#include <Button.h>
#include <Alert.h>

// Messages
#define COLOR_CHANGED 	'ccrq'
#define BUTTON_OK 		'btok'
#define BUTTON_UNDO		'btun'	

/*
* Constructor
* It is created with the dimensions of BRect
*/
ColorWindow :: ColorWindow (BRect frame, BHandler *handler)
			: BWindow (frame, "Change the background color", B_TITLED_WINDOW,B_NOT_RESIZABLE) {
	
	// Variables
	BPoint 				 leftTop(20.0, 50.0);
	color_control_layout matrix;
	long 				 cellSide;		
	
	BButton				 *okButton;
	BButton				 *undoButton;	
	
	// Create the  view and set the background color, then add child to window
	frame.OffsetTo(B_ORIGIN);
	fColorView = new ColorView (frame, "ColorView",handler);
	fColorView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(fColorView);
		   	   
	   	   
	fMessenger = new BMessenger(handler);
	   
	matrix = B_CELLS_16x16;
	cellSide = 16;
	   	   
	fColorControl = new BColorControl (leftTop, matrix, cellSide, "ColorControl");
	fColorView -> AddChild(fColorControl);
	
	okButton = new BButton (BRect(280, 320, 350, 350), "ok", "Change", new BMessage(BUTTON_OK));
	fColorView -> AddChild(okButton);
	
	undoButton = new BButton (BRect(200, 320, 270, 350), "undo", "Undo", new BMessage(BUTTON_UNDO));
	fColorView -> AddChild(undoButton);
	
	Show();
}

void ColorWindow :: MessageReceived (BMessage* message) {
	
	message->PrintToStream();
	
	switch (message -> what) {
	
		case BUTTON_OK: {
			rgb_color userColorChoice;
			BMessage  *msg;
	
			// I catch the color that was chosen by the user
			userColorChoice = fColorControl -> ValueAsColor();

			// Sending the message...
			msg = new BMessage (COLOR_CHANGED);
			msg -> AddInt8 ("red", (int8)userColorChoice.red);
			msg -> AddInt8 ("green", (int8)userColorChoice.green);
			msg -> AddInt8 ("blue", (int8)userColorChoice.blue);
			fMessenger->SendMessage(msg);
		}
		break;
				
		case BUTTON_UNDO: {
				
			BAlert *alert = new BAlert("", "Latest changes will be discarded", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
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

// Function that is activated when I close the window
bool ColorWindow :: QuitRequested() {
	return (true);
}
