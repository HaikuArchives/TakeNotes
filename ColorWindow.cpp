/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 14th May 2009
 *
 * Description: TODO
 */

#include "ColorWindow.h"

// Messages
#define COLOR_CHANGED 'ccrq'

// Constructor
// It is created with the dimensions of BRect
ColorWindow :: ColorWindow (BRect frame, BHandler *handler)
			: BWindow (frame, "Change the background color", B_TITLED_WINDOW,B_NOT_RESIZABLE) {
	// Variables
	BPoint 				leftTop(20.0, 50.0);
	color_control_layout matrix;
	long cellSide;			
	
	frame.OffsetTo(B_ORIGIN);
	fColorView = new ColorView (frame, "ColorView",handler);
	AddChild(fColorView);
		   	   
	   	   
	fMessenger = new BMessenger(handler);
	   
	matrix = B_CELLS_16x16;
	cellSide = 16;
	   	   
	fColorControl = new BColorControl (leftTop, matrix, cellSide, "ColorControl");
	fColorView -> AddChild(fColorControl);
	
	Show();
}

// Function that is activated when I close the window
bool ColorWindow :: QuitRequested() {
	// Variables
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
	return (true);
}