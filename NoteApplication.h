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


#ifndef NOTE_APPLICATION_H
#define NOTE_APPLICATION_H

#include "NoteWindow.h"

#include <Application.h>


// Definizione della classe applicazione
class NoteApplication : public BApplication {

	public:
			NoteApplication();
	
	private:
		NoteWindow * fNoteWindow;
}; 

#endif