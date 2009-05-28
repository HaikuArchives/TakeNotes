/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Ilio Catallo, 28th May 2009
 *
 * Description: TODO
 */


#ifndef NOTE_APPLICATION_H
#define NOTE_APPLICATION_H

#include "NoteWindow.h"

#include <Application.h>
#include <String.h>

// Definition of the class
class NoteApplication : public BApplication {

	public:
	
						NoteApplication();
		virtual void	ArgvReceived(int32 argc, char** argv);
		virtual void	RefsReceived(BMessage *message);
		virtual void	MessageReceived(BMessage *message);
		virtual void	ReadyToRun();
		
				void 	OpenNote();
				void	OpenNote(entry_ref *ref);
				void	CloseNote();
				void	CheckMime();
	
	private:
	
		int32		fWindowCount;
		int32		fWindowCountUntitled;	

}; 

extern NoteApplication *note_app;

#endif