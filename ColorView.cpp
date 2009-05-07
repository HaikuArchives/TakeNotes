/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Ilio Catallo, 8th April 2009
 *
 * Description: TODO
 */

#include "ColorView.h"

// Messages
#define COLOR_CHANGED 'ccrq'

// Constructor
ColorView :: ColorView(BRect rect, char *name,BHandler *handler)
	   	  : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW){
	   	   // Variables
	   	   BPoint 				leftTop(20.0, 50.0);
	   	   color_control_layout matrix;
	   	   long cellSide;	   	   
	   	   
	   	   fMessenger = new BMessenger(handler);
	   	   
	   	   matrix = B_CELLS_16x16;
	   	   cellSide = 16;
	   	   
	   	   fColorControl = new BColorControl (leftTop, matrix, cellSide, "ColorControl");
	   	   AddChild(fColorControl);
}

// Font
void ColorView :: AttachedToWindow() {
	SetFont (be_bold_font);
	SetFontSize(12);
}

// Drawing the window
void ColorView :: Draw (BRect updateRect) {
	MovePenTo(BPoint(20.0, 20.0));
	DrawString ("Scegli!");
}

// When the user clicks...
void ColorView :: MouseDown(BPoint point) {
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
}