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


// Messaggi
#define COLOR_CHANGED 'ccrq'

// Costruttore
ColorView :: ColorView(BRect rect, char *name,BHandler *handler)
	   	  : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW){
	   	   
	   	   // Variabili
	   	   
	   	   fMessenger = new BMessenger(handler);
	   	   
	   	   BPoint leftTop(20.0, 50.0);
	   	   color_control_layout matrix = B_CELLS_16x16;
	   	   long cellSide = 16;
	   	   
	   	   fColorControl = new BColorControl (leftTop, matrix, cellSide, "ColorControl");
	   	   AddChild(fColorControl);

}

// Font
void ColorView :: AttachedToWindow() {
	SetFont (be_bold_font);
	SetFontSize(12);
}

// Disegno nella finestra
void ColorView :: Draw (BRect updateRect) {
	MovePenTo(BPoint(20.0, 20.0));
	DrawString ("Scegli!");
}

// Quando clicco...
void ColorView :: MouseDown(BPoint point) {
	// Variabili
	
	rgb_color userColorChoice;
	
	// Prendo il valore scelto dall'utente
	userColorChoice = fColorControl -> ValueAsColor();

	// Invio il messaggio
	BMessage *msg = new BMessage (COLOR_CHANGED);
	msg -> AddInt8 ("red", (int8)userColorChoice.red);
	msg -> AddInt8 ("green", (int8)userColorChoice.green);
	msg -> AddInt8 ("blue", (int8)userColorChoice.blue);
	fMessenger->SendMessage(msg);
}