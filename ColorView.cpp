/*
 * View di disegno
 *
 * Autore: Eleonora Ciceri
 * Data: 2 Aprile 2009
 */
 
// L'ho messo per inserire la scritta nel quadrato
#ifndef COLOR_VIEW_H
#include "ColorView.h"
#endif

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef _BUTTON_H
#include <Button.h>
#endif

// Messaggi
#define COLOR_CHANGED 'CC'

// Costruttore
ColorView :: ColorView(BRect rect, char *name)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW) {
	   	   // Variabili
	   	   BPoint leftTop(20.0, 50.0);
	   	   color_control_layout matrix = B_CELLS_16x16;
	   	   long cellSide = 16;
	   	   
	   	   fColorControl = new BColorControl (leftTop, matrix, cellSide, "ColorControl");
	   	   AddChild(fColorControl);
	   	   
	   	   BRect buttonRect (300,300,400,400);
	   	   BMessage *msg = new BMessage (COLOR_CHANGED);
	   	   fButton = new BButton (buttonRect, "MyButton", "clickMe", msg);
	   	   AddChild (fButton);
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
	BRect aRect (20.0, 330.0, 350.0, 340.0);
	rgb_color userColorChoice;
	
	// Prendo il valore scelto dall'utente
	userColorChoice = fColorControl -> ValueAsColor();
	SetHighColor(userColorChoice);
	FillRect (aRect, B_SOLID_HIGH);
	// Invio il messaggio
	BMessage *msg = new BMessage (COLOR_CHANGED);
	msg -> AddInt8 ("red", (int8)userColorChoice.red);
	msg -> AddInt8 ("green", (int8)userColorChoice.green);
	msg -> AddInt8 ("blue", (int8)userColorChoice.blue);
	be_app->PostMessage(msg);
}