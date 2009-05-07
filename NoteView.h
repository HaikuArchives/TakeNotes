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
 * Last revision: Ilio Catallo, 17th April 2009
 *
 * Description: TODO
 */


#ifndef NOTE_VIEW_H
#define NOTE_VIEW_H

#include <View.h>
#include <TextView.h>
#include <Messenger.h>
#include <String.h>


class NoteView : public BTextView {

	public:
						
							NoteView (BRect frame, BRect frameText, char *name,BHandler *handler);
							NoteView (BMessage *msg);
	   				   	   ~NoteView();
	   		void			Init();
	static 	BArchivable*	Instantiate(BMessage *msg);
			void			AboutRequested();	   
	    	status_t		Archive(BMessage *msg, bool deep=true) const;
			void			MessageReceived(BMessage *message);
		
			void 			SetBackgroundColor(rgb_color color);
	virtual	void 			InsertText(const char *text, int32 length, int32 offset, const text_run_array *runs=NULL);

	private:
	
		BHandler 	*handler;
		BMessenger 	*fMessenger;
		BString 	*message;
	

};

#endif