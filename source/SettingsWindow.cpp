/*
 * Copyright 2020, thaflo, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			thaflo
 *
 * December 2020: thaflo: Change color directly (that's the Be way)
 */

#include "SettingsWindow.h"
#include <ColorControl.h>
#include <stdio.h>

// Libraries
#include <Alert.h>
#include <Button.h>
#include <private/shared/CalendarView.h>

// Messages
#define COLOR_CHANGED 	'ccrq'
#define REVERT_COLOR		'revc'
#define DEFAULT_COLOR		'defc'
#define SETTINGS_CLOSE		'_clc'
#define COLOR_CHOSEN		'_cch'
#define SAVE		'save'

//translation
#include <Catalog.h>
#include <TranslationUtils.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SettingsWindow"

using namespace BPrivate;

/*
* Constructor
* It is created with the dimensions of BRect
*/
SettingsWindow :: SettingsWindow (BRect frame, BHandler *handler, rgb_color color)
			: BWindow (frame, B_TRANSLATE("Note settings"), B_TITLED_WINDOW, B_NOT_RESIZABLE) {

	// Variables
	BPoint 				 leftTop(10.0, 50.0);
	color_control_layout matrix;
	long 				 cellSide;

	BButton				 *revertButton;
	BButton				*setDefButton;

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
	fColorControl -> SetValue(color);
	fColorView -> AddChild(fColorControl);

	revertButton = new BButton (BRect(10, 150, 170, 175), "default", B_TRANSLATE("Default"), new BMessage(REVERT_COLOR));
	revertButton->ResizeToPreferred();
	fColorView -> AddChild(revertButton);

	setDefButton = new BButton (BRect(180, 150, 340, 175), "default_color", B_TRANSLATE("set as default"), new BMessage(DEFAULT_COLOR));
	setDefButton->ResizeToPreferred();
	fColorView -> AddChild(setDefButton);

	BCalendarView *fCalendarView = new BCalendarView(BRect(10, 180, 210, 300), "calendarView", B_FOLLOW_LEFT_TOP, B_WILL_DRAW | B_FRAME_EVENTS
										| B_NAVIGABLE | B_PULSE_NEEDED);
	fCalendarView->ResizeToPreferred();
	fColorView->AddChild(fCalendarView);
	printf("settings window\n");

	Show();
}

// Messages received by the SettingsWindow
void SettingsWindow :: MessageReceived (BMessage* message) {
	// Variables
	rgb_color 	userColorChoice;
	BMessage  	*msg;
	BAlert 		*alert;

	//message->PrintToStream();

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
		case REVERT_COLOR: {

			alert = new BAlert("", B_TRANSLATE("Set default values ?"), B_TRANSLATE("Yes"), B_TRANSLATE("No"), NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->SetShortcut(0, B_ESCAPE);

			if (alert->Go() == 0) {
				fColorControl->SetValue(fBgColor);
				this->PostMessage(COLOR_CHANGED);
			}
		}
		break;

		// It answer to an set new default color request
		case DEFAULT_COLOR: {

			alert = new BAlert("", B_TRANSLATE("All new notes will have this color?"), B_TRANSLATE("Yes"), B_TRANSLATE("No"), NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->SetShortcut(0, B_ESCAPE);

			if (alert->Go() == 0) {
				msg = new BMessage(DEFAULT_COLOR);
				msg -> AddColor("new_def_color", fColorControl -> ValueAsColor());
				fMessenger->SendMessage(msg);
			}
		}
		break;

		default:
			BWindow::MessageReceived(message);
		break;
	}
}

// Hook function-override, we should inform NoteWindow that SettingsWindow will be closed
void SettingsWindow :: Quit(){

	// Variables
	BMessage *message;

	// Inform NoteWindow that this window is going to be closed
	message = new BMessage (SETTINGS_CLOSE);

	fMessenger->SendMessage(message);

	// Execute the real code
	BWindow :: Quit();
}



// Function that is activated when I close the window
bool SettingsWindow :: QuitRequested() {
	//fMessenger->SendMessage(SAVE);
	Quit();
	return (true);
}
