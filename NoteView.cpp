/*
 * View di disegno
 *
 * Autore: Eleonora Ciceri, Stefano Celentano
 * Data: 2 Aprile 2009
 * Ultima revisione: Stefano Celentano, 7 Aprile 2009
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

#define 	TEXT_INSET 5.0


BRect	textViewFrame(0,10,260,260);

// Costruttore
NoteView :: NoteView(BRect rect, char *name)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW) {
	   	   	   	
	   	BRect textBounds;
	   	
	   	
	   	textBounds.left = textViewFrame.left + TEXT_INSET;
	   	textBounds.right = textViewFrame.right - textViewFrame.left - TEXT_INSET;
	   	textBounds.top = textViewFrame.top + TEXT_INSET;
	   	textBounds.bottom = textViewFrame.bottom - textViewFrame.top - TEXT_INSET;


		fTextView = new BTextView(textViewFrame, "TextView", textBounds, B_FOLLOW_NONE, B_WILL_DRAW);
		
		AddChild(fTextView);	
	   	
	   	
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
/*	
	MovePenTo(BPoint(10,40));
	DrawString("Testo:");
*/	
	fTextView->MakeFocus();
}

// Cambio il colore
void NoteView :: setColor (rgb_color colore) {
	//cambio il colore della TextView
	fTextView->SetViewColor(colore);
}
