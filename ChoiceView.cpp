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

#include "ChoiceView.h"

#include <Alert.h>
#include <StringView.h>
#include <Roster.h>
#include <RadioButton.h>
#include <Message.h>
#include <String.h>
#include <Window.h>
#include <Entry.h>
#include <stdio.h>

#define RADIO_CHECKED 	'rdck'
#define BUTTON_OK		'btok'
#define BUTTON_UNDO		'btun'

// Constructor
ChoiceView :: ChoiceView(BRect rect, char *name,BHandler *handler)
	   	  : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS){
	 
	 	  //Create a Messenger and set its target as NoteWindow
	 	  fMessenger = new BMessenger(handler);
}

// Font
void ChoiceView :: AttachedToWindow() {
	SetFont (be_bold_font);
	SetFontSize(12);
}

// Drawing the window
void ChoiceView :: Draw (BRect updateRect) {
	
	// Variables
	BStringView 	*instructions;
	BRect 			frame(10,20,600,35);
	BString			name;
	BRadioButton	*button;
	BMessage		*message;
	
	BList			*aList;
	app_info		*appInfo;
	entry_ref		ref;
	team_id			who;	
	
	int 			count,
					x = 30,
					y = 40;
	
	const char *text = "Choose the application that is related to the note:";

	//Show a little instruction about what to do
	instructions = new BStringView (frame, "instructions", text);
	instructions -> SetFont(be_bold_font);
	instructions -> SetFontSize(12);
	AddChild(instructions);	
					
	// Initialization
	aList = new BList;
    appInfo = new app_info();

   	// Obtaining the applications that are running
   	be_roster->GetAppList(aList); 

	// We look for the current application instances that are running
	for (count=0;count< aList->CountItems();count++){

   	    who = (team_id)aList->ItemAt(count);
   	  	be_roster->GetRunningAppInfo(who,appInfo);
   	  	
   	  	// Show the name of the running program istead of its signature
   		ref = appInfo->ref;
   		name.SetTo(ref.name);
   	  	
   	  	//not show the system process like the *_server programs or the Deskbar / Tracker
   		if (name.Compare("registrar") != 0 && name.Compare("Tracker") != 0 && name.Compare("Deskbar") != 0 &&
   			name.Compare("net_server") != 0 && name.Compare("syslog_daemon") !=0 && name.Compare("input_server") != 0 &&
   			name.Compare("midi_server") !=0 && name.Compare("print_server") !=0 && name.Compare("media_server") != 0 &&
   			name.Compare("media_addon_server") != 0 && name.Compare("afp_server") != 0 && name.Compare("debug_server") != 0){
   	  

				//Create the message for the current radio button and fills it with the application signature
 	 	 	  	message = new BMessage (RADIO_CHECKED);
   	  			message -> AddString("signature", appInfo -> signature);
   	  	
   	  			// Show a new Radio Button and place it on the view
   	  			frame.Set(x, y, 300, 20);
   	  			button = new BRadioButton(frame, appInfo -> signature, ref.name, message);
   	  			AddChild(button);
   	  	
   	  			y += 20;
   	  	
   	  	}
      	
      	
    }
    
    //Add the Ok and the Undo Button
    fUndoButton = new BButton(BRect(x,y,x+50,y+10),"undo","Undo",new BMessage(BUTTON_UNDO));
   	fOkayButton = new BButton(BRect(x+70,y,x+120,y+10),"ok","OK",new BMessage(BUTTON_OK));
  
  	//Make them children
  	AddChild(fUndoButton);
  	AddChild(fOkayButton);
  

}

/*
// When the user clicks...
void ChoiceView :: MouseDown(BPoint point) {
	
}
*/

void ChoiceView :: MessageReceived(BMessage* message) {
	
	message->PrintToStream();
	
	switch(message->what){
	/*
		case RADIO_CHECKED:{
		
			fCurrentCheckedMessage = message;
			
		}
		break;
		
		case BUTTON_OK:{
		
			fMessenger->SendMessage(fCurrentCheckedMessage);
			Window()->PostMessage(B_QUIT_REQUESTED);
		
		}
		break;
		
		case BUTTON_UNDO:{
		
			BAlert* alert = new BAlert("", "Do you really want to close the window ?", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->SetShortcut(0, B_ESCAPE);

			if (alert->Go() == 0) {
				//Discard all the changes
				Window()->PostMessage(B_QUIT_REQUESTED);
			}
	
		}*/
		default:
			BView::MessageReceived(message);
		break;
	
	}

}