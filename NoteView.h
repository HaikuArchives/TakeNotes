/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Stefano Celentano
 *			Eleonora Ciceri
 * 
 * Last revision: Ilio Catallo, 28th May 2009
 *
 * Description: TODO
 */


#ifndef NOTE_VIEW_H
#define NOTE_VIEW_H

#include "NoteText.h"

#include <Messenger.h>
#include <String.h>
#include <View.h>

class _EXPORT NoteView;

class NoteView : public BView {

	public:
						
							NoteView (BRect frame, BHandler *handler);
							NoteView (BMessage *msg);
	   				   	   ~NoteView(); 		
	static 	BArchivable*	Instantiate(BMessage *msg);
			void			AboutRequested();	   
	    	status_t		Archive(BMessage *msg, bool deep=true) const;
			void			MessageReceived(BMessage *message);
		
			void 			SetBackgroundColor(rgb_color color);

	private:
	
		bool		fReplicated;
		NoteText	*fNoteText;
		BMessenger 	*fMessenger;
		BString 	*message;
	

};

#endif