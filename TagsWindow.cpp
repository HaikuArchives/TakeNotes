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

#include <View.h>
#include <Entry.h>
#include <Directory.h>
#include <Alert.h>
#include <Message.h>

#include <stdio.h>
#include <stdlib.h>

//Constants
#define BUTTON_OK 		'btok'
#define BUTTON_UNDO 	'btun'


TagsWindow :: TagsWindow(BMessage *fSaveMessage)
		   : BWindow (BRect(300,300,700,550),"Set Tags for this note",B_TITLED_WINDOW, B_NOT_RESIZABLE){
		   
		//Variables
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
		 		
		// Create the Text Field 
		fTag1 = new BTextControl(BRect(20,40,200,55), "tag1", "First Tag: ", NULL, NULL);
		fTag2 = new BTextControl(BRect(20,70,200,85), "tag2", "Second Tag: ", NULL, NULL);
		fTag3 = new BTextControl(BRect(20,100,200,135), "tag3", "Third Tag: ", NULL, NULL);

		//Create the OK and UNDO button	
		fDoneButton = new BButton(BRect(340,200,390,210), "ok", "OK", new BMessage(BUTTON_OK));
		fUndoButton = new BButton(BRect(270,200,320,210), "undo", "Undo", new BMessage(BUTTON_UNDO));
		
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
		fTagsView->AddChild(fDoneButton);
		fTagsView->AddChild(fUndoButton);
		
	
		// Open the file from FS starting from the fSaveMessage message
		if (fSaveMessage->FindRef("directory",&ref) == B_OK && fSaveMessage->FindString("name", &name) == B_OK){
			
			dir.SetTo(&ref);
			if ((err = dir.InitCheck()) != B_OK){
			 BAlert *myalert = new BAlert("ERR","errore di inizializzazione del file","OK");
			 myalert->Go();
			 exit(-1);	
			
			}
			fFile.SetTo(&dir, name, B_READ_WRITE);
		}
		
		printf("$$$$TAGONE$$$$: %s\n", bufferTag1);
		
		// Read the old values for this file
		fFile.ReadAttr("TAKENOTES:tagone", B_STRING_TYPE, 0, &bufferTag1, 30);
		fFile.ReadAttr("TAKENOTES:tagtwo", B_STRING_TYPE, 0, &bufferTag2, 30);
		fFile.ReadAttr("TAKENOTES:tagthree", B_STRING_TYPE, 0, &bufferTag3, 30);
			
		// Initialize text field to old values
		fTag1->SetText(bufferTag1);
		fTag2->SetText(bufferTag2);
		fTag3->SetText(bufferTag3);		

}

TagsWindow :: ~TagsWindow(){

	 if (fFile.InitCheck() == B_OK)
			fFile.Unset();
		
}

void TagsWindow :: MessageReceived(BMessage *message){

	switch(message->what){
		case BUTTON_OK: {
																			
			// Set each attribute to text field's content
			fFile.WriteAttr("TAKENOTES:tagone", B_STRING_TYPE, 0, fTag1->Text(), 30); 					
			fFile.WriteAttr("TAKENOTES:tagtwo", B_STRING_TYPE, 0, fTag2->Text(), 30); 
			fFile.WriteAttr("TAKENOTES:tagthree", B_STRING_TYPE, 0, fTag3->Text(), 30);
			 
			// Closing window
			Quit();		
																											
		}
		break;
				
		case BUTTON_UNDO: {
				
			BAlert *alert = new BAlert("", "The tags haven't been saved yet, do you really want to close the window ?", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
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