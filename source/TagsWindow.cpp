/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Ilio Catallo,
 *			Stefano Celentano
 *
 * Last revision: Ilio Catallo, 28th June 2009
 *
 * Description: Tags window, it allows the user to set three customs tags (extra attributes)
 */


#include "TagsWindow.h"

#include <Alert.h>
#include <Directory.h>
#include <Entry.h>
#include <Message.h>
#include <View.h>
#include <Catalog.h>
#include <TranslationUtils.h>

#include <stdio.h>
#include <stdlib.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TagsWindow"

// Constants
#define BUTTON_OK 		'btok'
#define BUTTON_CANCEL 	'btcn'
#define TAGS_CLOSE		'_tgc'



TagsWindow :: TagsWindow(BMessage *fSaveMessage, BHandler *handler)
		   : BWindow (BRect(300,300,700,450), B_TRANSLATE("Set Tags for this note")
		   ,B_TITLED_WINDOW, B_NOT_RESIZABLE){

	// Variables
	fMessenger = new BMessenger(handler);

	BDirectory		dir;
	entry_ref		ref;
	status_t		err;
	const char		*name;

	BView			*fTagsView;
	char			bufferTag1[30];
	char			bufferTag2[30];
	char			bufferTag3[30];

	// Create the main view
	fTagsView = new BView(Bounds(), "TagsView", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS);

	fTag1 = new BTextControl(BRect(10,10,200,30), "tag1", B_TRANSLATE("First Tag: "), NULL, NULL);
	fTag2 = new BTextControl(BRect(10,40,200,60), "tag2", B_TRANSLATE("Second Tag: "), NULL, NULL);
	fTag3 = new BTextControl(BRect(10,70,200,90), "tag3", B_TRANSLATE("Third Tag: "), NULL, NULL);

	//Create the OK and UNDO button
	fCancelButton = new BButton(BRect(210,115,290,135), "cancel",
			B_TRANSLATE("Cancel"), new BMessage(BUTTON_CANCEL));
	fOkButton = new BButton(BRect(300,115,380,135), "ok",
			B_TRANSLATE("OK"), new BMessage(BUTTON_OK));

	/*
	* Add the view as window's child, set back ground color,
	* then add the tag TextControl as children
	*/
	fTagsView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	AddChild(fTagsView);
	fTagsView->AddChild(fTag1);
	fTagsView->AddChild(fTag2);
	fTagsView->AddChild(fTag3);

	// Add OK and UNDO button as children
	fTagsView->AddChild(fOkButton);
	fTagsView->AddChild(fCancelButton);

	// Open the file from FS starting from the fSaveMessage message
	if (fSaveMessage->FindRef("directory",&ref) == B_OK && fSaveMessage->FindString("name", &name) == B_OK){

		dir.SetTo(&ref);
		if ((err = dir.InitCheck()) != B_OK){
			BAlert *myalert = new BAlert("ERR", B_TRANSLATE("File initiation failed"), B_TRANSLATE("OK"));
			myalert->Go();
			exit(-1);

		}
		fFile.SetTo(&dir, name, B_READ_WRITE);
	}

	printf("$$$$TAGONE$$$$: %s\n", bufferTag1);

	// Read the old values for this file
	if (fFile.ReadAttr("TAKENOTES:tagone", B_STRING_TYPE, 0, &bufferTag1, 30) > 0) {
		fTag1->SetText(bufferTag1);
	}

	if (fFile.ReadAttr("TAKENOTES:tagtwo", B_STRING_TYPE, 0, &bufferTag2, 30) > 0) {
		fTag2->SetText(bufferTag2);
	}

	if (fFile.ReadAttr("TAKENOTES:tagthree", B_STRING_TYPE, 0, &bufferTag3, 30) > 0) {
		fTag3->SetText(bufferTag3);
	}

}

// Destructor
TagsWindow :: ~TagsWindow(){

	if (fFile.InitCheck() == B_OK)
		fFile.Unset();

}

// FUnction that received all the messages
void TagsWindow :: MessageReceived(BMessage *message){

	switch(message->what){
		// Message that answer to an OK request
		case BUTTON_OK: {

			// Set each attribute to text field's content
			fFile.WriteAttr("TAKENOTES:tagone", B_STRING_TYPE, 0, fTag1->Text(), 30);
			fFile.WriteAttr("TAKENOTES:tagtwo", B_STRING_TYPE, 0, fTag2->Text(), 30);
			fFile.WriteAttr("TAKENOTES:tagthree", B_STRING_TYPE, 0, fTag3->Text(), 30);

			// Closing window
			Quit();
		}
		break;

		// Message that answer to an UNDO request
		case BUTTON_CANCEL: {
			Quit();
		}
		break;

		default:
			BWindow::MessageReceived(message);
		break;
	}

}

// Hook function-override, we should inform NoteWindow that TagsWindow will be closed
void TagsWindow :: Quit(){

	// Variables
	BMessage *message;

	// Inform NoteWindow that this window is going to be closed
	message = new BMessage (TAGS_CLOSE);
	fMessenger->SendMessage(message);

	// Execute the real code
	BWindow :: Quit();

}

// Function that answer to a QUIT requested
bool TagsWindow :: QuitRequested() {

	Quit();
	return true;

}
