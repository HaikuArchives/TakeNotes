/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 * 
 * Last revision: Ilio Catallo, 13th May 2009
 *
 * Description: TODO
 */
 
 
#ifndef NOTE_TEXT_H
#define NOTE_TEXT_H
 
#include <TextView.h>
#include <Messenger.h>
 
class NoteText : public BTextView {
 
 	public:
 	
 								NoteText(BRect frame, BRect frameText, char *name,BHandler *handler);
 								NoteText(BMessage *message);
 		static	BArchivable*	Instantiate(BMessage *msg);
 		virtual void			InsertText(const char *text, int32 length, int32 offset, const text_run_array *runs=NULL);
 				status_t		Archive(BMessage *msg, bool deep=true) const;
 			
 		
 	private:
 	
 		bool		fReplicated;
 		BHandler	*handler;
 		BMessenger	*fMessenger;
};
 
 
#endif		
 
 