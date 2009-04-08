/*
 * Prova applicazione per il trasferimento di messaggi
 *
 * Autori: Eleonora Ciceri, Ilio Catallo
 * Data: 2 Aprile 2009
 */

// Dichiarazione degli header
#ifndef NOTE_APPLICATION_H
#define NOTE_APPLICATION_H

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef NOTE_WINDOW_H
#include "NoteWindow.h"
#endif

// Definizione della classe applicazione
class NoteApplication : public BApplication {

	public:
			NoteApplication();
	
	private:
		NoteWindow * fNoteWindow;
}; 

#endif