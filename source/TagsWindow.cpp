/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Ilio Catallo,
 *			Stefano Celentano
 *
 * Last revision: Florian Thaler, 08.04.2021
 *
 * Description: Tags window, it allows the user to set three customs tags (extra attributes)
 */


#include "TagsWindow.h"

#include <Alert.h>
#include <Directory.h>
#include <Entry.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <LayoutBuilder.h>
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
		   : BWindow (BRect(300,300,700,450), B_TRANSLATE("Set tags for this note")
		   ,B_TITLED_WINDOW, B_NOT_RESIZABLE){

	// Variables
	fMessenger = new BMessenger(handler);

	BDirectory		dir;
	entry_ref		ref;
	status_t		err;
	const char		*name;

	BString tagone;
	BString tagtwo;
	BString tagthree;

	fTag1 = new BTextControl("tag1", B_TRANSLATE("First tag: "), NULL, NULL);
	fTag2 = new BTextControl("tag2", B_TRANSLATE("Second tag: "), NULL, NULL);
	fTag3 = new BTextControl("tag3", B_TRANSLATE("Third tag: "), NULL, NULL);

	fCancelButton = new BButton("cancel", B_TRANSLATE("Cancel"), new BMessage(BUTTON_CANCEL));
	fOkButton = new BButton("ok", B_TRANSLATE("OK"), new BMessage(BUTTON_OK));

	//thaflo, 2021, adding layout management
	SetLayout(new BGroupLayout(B_VERTICAL));
	BView* fTopView = new BGroupView(B_VERTICAL);

	BLayoutBuilder::Group<>(fTopView, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_VERTICAL)
			.Add(fTag1)
			.Add(fTag2)
			.Add(fTag3)
		.End()
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(fCancelButton)
			.Add(fOkButton)
	.End();
	AddChild(fTopView);

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

	fFile.ReadAttrString("TAKENOTES:tagone", &tagone);
	fFile.ReadAttrString("TAKENOTES:tagtwo", &tagtwo);
	fFile.ReadAttrString("TAKENOTES:tagthree", &tagthree);

	fTag1->SetText(tagone.String());
	fTag2->SetText(tagtwo.String());
	fTag3->SetText(tagthree.String());

	Show();
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
