/*
 * View di disegno
 *
 * Autore: Eleonora Ciceri, Stefano Celentano
 * Data: 2 Aprile 2009
 * Ultima revisione: Ilio Catallo, 9 Aprile 2009
 */

// Messaggi
#define FONT_BOLD 'fntb'

#ifndef NOTE_VIEW_H
#include "NoteView.h"
#endif

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

#ifndef _APPLICATION_H
#include <Application.h>
#endif

// Messaggi
#define TEXT_CHANGED 'txch'

// Costruttore
NoteView :: NoteView(BRect frame,BRect frameText, char *name, BHandler *handler )
	   	   : BTextView(frame, name, frameText, B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW) {
	   	   
	   	   rgb_color giallo = {254,254,92,255};
	   	   SetViewColor(giallo);
	   	   
	   	   fMessenger = new BMessenger(handler);
	   	   
}


void NoteView :: SetBackgroundColor (rgb_color colore) {

	SetViewColor(colore);
	Invalidate();
		
}


void NoteView :: InsertText(const char* text, int32 length, int32 offset, const text_run_array *runs) {

    BMessage *msg = new BMessage (TEXT_CHANGED);
	fMessenger->SendMessage(msg);	
	BTextView::InsertText(text,length,offset,runs);

}