/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 13th May 2009
 *
 * Description: TODO
 */

#include "ChoiceView.h"

#include <StringView.h>
#include <Roster.h>
#include <RadioButton.h>
#include <stdio.h>

#define RADIO_CHECKED 'rdck'

// Constructor
ChoiceView :: ChoiceView(BRect rect, char *name,BHandler *handler)
	   	  : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW){
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
	BList			*aList;
	app_info		*appInfo;	
	int 			count,
					x = 30,
					y = 40;
	team_id			who;
	BRadioButton	*button;
	BMessage		*message;
	
	const char *text = "Choose the application that is related to the replicant:";

	instructions = new BStringView (frame, "instructions", text);
	instructions -> SetFont(be_bold_font);
	instructions -> SetFontSize(12);
	AddChild(instructions);	
	
					
	// Initialization
	aList = new BList;
    appInfo = new app_info();

   	// Obtaining the applications that are running
   	be_roster->GetAppList(aList); 

	// We look for the current instances of BeZilla that are running
	for (count=0;count< aList->CountItems();count++){

   	    who = (team_id)aList->ItemAt(count);
   	  	be_roster->GetRunningAppInfo(who,appInfo);
   	  	frame.Set(x, y, 300, 20);
   	  	
   	  	printf("%s\n", appInfo -> signature);
   	  	
   	  	message = new BMessage (RADIO_CHECKED);
   	  	message -> AddString("signature", appInfo -> signature);
   	  	
   	  	button = new BRadioButton(frame, appInfo -> signature, appInfo -> signature, message);
   	  	AddChild(button);
   	  	
   	  	y += 20;
    }
    // RadioButton for the Desktop
    frame.Set(x, y, 300, 20);
    message = new BMessage(RADIO_CHECKED);
    message -> AddString("signature", "Desktop");
    button = new BRadioButton(frame, "Desktop", "Desktop", message);
    AddChild(button);
}

// When the user clicks...
void ChoiceView :: MouseDown(BPoint point) {
	
}

void ChoiceView :: MessageReceived(BMessage* message) {
	message->PrintToStream();
}