/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 * 
 * Last revision: Ilio Catallo, 14th May 2009
 *
 * Description: TODO
 */
 
 
#ifndef NOTE_TEXT_H
#define NOTE_TEXT_H
 
#include <TextView.h>
#include <Messenger.h>
#include <Bitmap.h>
#include <TranslationUtils.h>
#include <Path.h>
#include <Entry.h>
#include <Window.h> 
#include <stdio.h>
 
 
class NoteText : public BTextView {
 
 	public:
 	
 								NoteText(BRect frame, BRect frameText, char *name,BHandler *handler);
 								NoteText(BMessage *message);
 							 	~NoteText();
 		static	BArchivable*	Instantiate(BMessage *msg);
 		virtual void			InsertText(const char *text, int32 length, int32 offset, const text_run_array *runs=NULL);
 		virtual	void			Draw(BRect updateRect);
 		virtual	void			MessageReceived(BMessage *message);
 		virtual	void			KeyDown(const char* bytes, int32 numBytes);		
 		virtual void			FrameResized(float width, float height);		
 				
 				status_t		Archive(BMessage *msg, bool deep=true) const;
 			
 		
 	private:
 	
 		bool		fReplicated;
 		BHandler	*handler;
 		BMessenger	*fMessenger;
 		//BBitmap		*fBitmap;
};
 
 
#endif		
 
 