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
 * December 2020: thaflo: Change color directly (that's the Be way)
 * Description: Window that is opened to change the background color
 */

#include "ColorWindow.h"
#include "NoteText.h"
#include <ColorControl.h>
#include <stdio.h>

// Libraries
#include <Alert.h>
#include <Button.h>

// Messages
#define COLOR_CHANGED 	'ccrq'
#define BUTTON_DEFAULT		'btdf'
#define COLOR_CLOSE		'_clc'
#define COLOR_CHOSEN		'_cch'
#define COLOR 'colo'
#define SAVE 'save'

//translation
#include <Catalog.h>
#include <TranslationUtils.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ColorWindow"

/*
* Constructor
* It is created with the dimensions of BRect
*/
ColorWindow :: ColorWindow (BRect frame, BHandler *handler, rgb_color color)
			: BWindow (frame, B_TRANSLATE("Change the background color"), B_TITLED_WINDOW, B_NOT_RESIZABLE) {

	// Variables
	BPoint 				 leftTop(10.0, 50.0);
	color_control_layout matrix;
	long 				 cellSide;

	BButton				 *undoButton;

	// Create the  view and set the background color, then add child to window
	frame.OffsetTo(B_ORIGIN);
	fColorView = new ColorView (frame, "ColorView",handler);
	fColorView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fColorView->ResizeToPreferred();
	AddChild(fColorView);

	// Drawing...
	fMessenger = new BMessenger(handler);

	matrix = B_CELLS_32x8;
	cellSide = 9.0;

	fColorControl = new BColorControl (leftTop, matrix, cellSide, "ColorControl", new BMessage(COLOR_CHANGED));
	fColorControl->SetValue(color);
	fColorView -> AddChild(fColorControl);

	undoButton = new BButton (BRect(10, 150, 170, 175), "default", B_TRANSLATE("Default"), new BMessage(BUTTON_DEFAULT));
	undoButton->ResizeToPreferred();
	fColorView -> AddChild(undoButton);

	Show();
}

// Messages received by the ColorWindow
void ColorWindow :: MessageReceived (BMessage* message) {
	// Variables
	rgb_color 	userColorChoice;
	BMessage  	*msg;
	BAlert 		*alert;

	message->PrintToStream();

	switch (message -> what) {
			case COLOR_CHANGED: {
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

		// It answer to an UNDO request
		case BUTTON_DEFAULT: {

			alert = new BAlert("", B_TRANSLATE("Set default values ?"), B_TRANSLATE("Yes"), B_TRANSLATE("No"), NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->SetShortcut(0, B_ESCAPE);

			if (alert->Go() == 0) {
				fColorControl->SetValue(gBgColor);
				this->PostMessage(COLOR_CHANGED);
			}
		}
		break;

		default:
			BWindow::MessageReceived(message);
		break;
	}
}

// Hook function-override, we should inform NoteWindow that ColorWindow will be closed
void ColorWindow :: Quit(){

	// Variables
	BMessage *message;

	// Inform NoteWindow that this window is going to be closed
	fMessenger->SendMessage(message);
	message = new BMessage (COLOR_CLOSE);
	fMessenger->SendMessage(message);

	// Execute the real code
	BWindow :: Quit();
}



// Function that is activated when I close the window
bool ColorWindow :: QuitRequested() {

	Quit();
	return (true);
}
