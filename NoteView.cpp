/*
 * View di disegno
 *
 * Autore: Eleonora Ciceri
 * Data: 2 Aprile 2009
 * Ultima revisione: Ilio Catallo, 5 Aprile 2009
 */
 
// L'ho messo per inserire la scritta nel quadrato
#ifndef NOTE_VIEW_H
#include "NoteView.h"
#endif

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

#ifndef _APPLICATION_H
#include <Application.h>
#endif

// Costruttore
NoteView :: NoteView(BRect rect, char *name)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW) {
}

// Font
void NoteView :: AttachedToWindow() {
	SetFont (be_bold_font);
	SetFontSize(12);
}

// Per disegnare la stringa!
void NoteView :: Draw (BRect updateRect) {
	// Variabili
	BRect frame = Bounds();
	//rgb_color colore = {15,255,255,255};
	
	
	StrokeRect(frame);
	
	MovePenTo(BPoint(10,40));
	DrawString("Testo:");
}

// Cambio il colore
void NoteView :: setColor (rgb_color colore) {
	SetViewColor(colore);
}