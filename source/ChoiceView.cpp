/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Ilio Catallo, 28th June 2009
 *
 * Description: ChoiceView is the view of the ChoiceWindow. It allows you to see the list of applications.
 */

// Libraries
#include "ChoiceView.h"

#include <Alert.h>
#include <Entry.h>
#include <Message.h>
#include <RadioButton.h>
#include <Roster.h>
#include <String.h>
#include <StringView.h>
#include <Window.h>
#include <stdio.h>

// Messages
#define RADIO_CHECKED 	'rdck'
#define BUTTON_OK		'btok'
#define BUTTON_UNDO		'btun'

// Constructor
ChoiceView :: ChoiceView(BRect rect, char *vname,BHandler *handler)
	   	  : BView(rect, vname, B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE){
	   	  
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

	// Show a little instruction about what to do
	instructions = new BStringView (frame, "instructions", text);
	instructions->SetFont(be_bold_font);
	instructions->SetFontSize(12);
	AddChild(instructions);	
					
	// Initialization
	aList = new BList;
    appInfo = new app_info();

   	// Obtaining the applications that are running
   	be_roster->GetAppList(aList); 

	// We look for the current application instances that are running
	for (count=0;count< aList->CountItems();count++){

   	    who = (team_id)(addr_t)aList->ItemAt(count);
   	  	be_roster->GetRunningAppInfo(who,appInfo);
   	  	
   	  	// Show the name of the running program istead of its signature
   		ref = appInfo->ref;
   		name.SetTo(ref.name);
   	  	
   	  	// Not show the system process like the *_server programs or the Deskbar / Tracker
   		if (name.Compare("registrar") != 0 && name.Compare("Tracker") != 0 && name.Compare("Deskbar") != 0 &&
   			name.Compare("net_server") != 0 && name.Compare("syslog_daemon") !=0 && name.Compare("input_server") != 0 &&
   			name.Compare("midi_server") !=0 && name.Compare("print_server") !=0 && name.Compare("media_server") != 0 &&
   			name.Compare("media_addon_server") != 0 && name.Compare("afp_server") != 0 && name.Compare("debug_server") != 0){
   	  

				// Create the message for the current radio button and fills it with the application signature
 	 	 	  	message = new BMessage (RADIO_CHECKED);
   	  			message->AddString("signature", appInfo->signature);
   	  	
   	  			// Show a new Radio Button and place it on the view
   	  			frame.Set(x, y, 300, 20);
   	  			button = new BRadioButton(frame, appInfo->signature, ref.name, message);
   	  			AddChild(button);
   	  	
   	  			y += 20;   	  	
   	  	}      	
    }
    
    // Add the Ok and the Undo Button
    frame = Bounds();
    frame.left += 350;
    frame.top = frame.bottom - 40;
    frame.right-= 90;
    frame.bottom -= 10;
    fUndoButton = new BButton(frame,"undo","Undo",new BMessage(BUTTON_UNDO));
   	
   	frame = Bounds();
    frame.left += 430;
    frame.top = frame.bottom - 40;
    frame.right-= 10;
    frame.bottom -= 10;
   	fOkayButton = new BButton(frame,"ok","OK",new BMessage(BUTTON_OK));
  
  	// Make them children
  	AddChild(fUndoButton);
  	AddChild(fOkayButton);
	 
	//Create a Messenger and set its target as NoteWindow
	fMessenger = new BMessenger(handler);
}

// Font
void ChoiceView :: AttachedToWindow() {
	SetFont (be_bold_font);
	SetFontSize(12);
}
