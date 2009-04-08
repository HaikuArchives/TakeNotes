/*
 * Implementazione dell'applicazione
 *
 * Autori: Eleonora Ciceri, Ilio Catallo
 * Data: 2 Aprile 2009
 * Ultima revisione: Ilio Catallo, 9 Aprile 2009
 */
 
// Definizione delle inclusioni
#ifndef NOTE_WINDOW_H
#include "NoteWindow.h"
#endif

#ifndef NOTE_APPLICATION_H
#include "NoteApplication.h"
#endif 

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

#ifndef NOTE_VIEW_H
#include "NoteView.h"
#endif

#ifndef _VIEW_H
#include <View.h>
#endif

#ifndef _WINDOW_H
#include <Window.h>
#endif

#define COLOR_CHANGED 'ccrq'

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

