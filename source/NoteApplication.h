/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 *
 * Last revision: Ilio Catallo, 21th June 2009
 *
 * Description: Header related to the application
 */


#ifndef NOTE_APPLICATION_H
#define NOTE_APPLICATION_H

#include "NoteWindow.h"

#include <Application.h>
#include <String.h>
#include <image.h>


// Definition of the class
class NoteApplication : public BApplication {

	public:

							NoteApplication();
		virtual void		ArgvReceived(int32 argc, char** argv);
		virtual void		RefsReceived(BMessage *message);
		virtual void		MessageReceived(BMessage *message);
		virtual void		ReadyToRun();

				void		_InstallReplicantInDeskbar();

				void 		OpenNote();
				void		OpenNote(entry_ref *ref);
				void		CloseNote();
				status_t	CheckMime();

	private:

		int32		fWindowCount;
		int32		fWindowCountUntitled;

};

extern NoteApplication *note_app;

extern const char 	*kSignature;
extern const char 	*kDeskbarItemName;

status_t our_image(image_info &image);

#endif
