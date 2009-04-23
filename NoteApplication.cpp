/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 9th April 2009
 *
 * Description: TODO
 */

/*
 * Implementazione dell'applicazione
 *
 * Autori: Eleonora Ciceri, Ilio Catallo
 * Data: 2 Aprile 2009
 * Ultima revisione: Ilio Catallo, 9 Aprile 2009
 */
 

#include "NoteApplication.h"

#define COLOR_CHANGED 'ccrq'
#define FONT_BOLD 'fntb'

// Main
int main(){

	NoteApplication myApp;
	
	myApp.Run();
	return(0);
}


NoteApplication::NoteApplication()
	: BApplication("application/x-vnd.ccc-TakeNotes"){	
	
	// Variabili
	BRect	aRect;
	
	// Creazione del rettangolo che ridimensiona la finestra
	// x, y, w, h
	aRect.Set(30, 30, 300, 300);
	fNoteWindow = new NoteWindow(aRect);	
	
	// Mostro la finestra
	fNoteWindow->Show();	
}

