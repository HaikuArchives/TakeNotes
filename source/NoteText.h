/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Ilio Catallo
 *
 * Last revision: Ilio Catallo, 29th June 2009
 *
 * Description: Header of the NoteText
 */


#ifndef NOTE_TEXT_H
#define NOTE_TEXT_H

// Libraries
#include <Bitmap.h>
#include <Entry.h>
#include <Messenger.h>
#include <Path.h>
#include <TextView.h>


// Definition of the class
class NoteText : public BTextView {

 	public:

 								NoteText(BRect frame, BRect frameText, char *name,BHandler *handler);
 								NoteText(BMessage *message);
 							 	~NoteText();
 		static	BArchivable*	Instantiate(BMessage *msg);
 		virtual void			InsertText(const char *text, int32 length, int32 offset, const text_run_array *runs=NULL);

 		virtual	void			MessageReceived(BMessage *message);

 		virtual void			FrameResized(float width, float height);
 				status_t		Archive(BMessage *msg, bool deep=true) const;

 				void			SetReplicated(bool flag);
 				bool			GetReplicated();
 				void			SetHandler(BHandler *handler);


 	private:

 		bool		fReplicated;
 		BHandler	*handler;
 		BMessenger	*fMessenger;
};

#endif
