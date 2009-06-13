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
 * Last revision: Ilio Catallo, 13th June 2009
 *
 * Description: TODO
 */


#ifndef NOTE_VIEW_H
#define NOTE_VIEW_H

#include "AppHashTable.h"
#include "NoteText.h"

#include <File.h>
#include <Messenger.h>
#include <String.h>
#include <View.h>

class _EXPORT NoteView;

class NoteView : public BView {

	public:
						
							NoteView (BRect frame,int32 resizingMode, bool inDeskbar = false, BHandler *handler = NULL);
							NoteView (BMessage *msg, BHandler *handler = NULL);
	   				   	   ~NoteView(); 		
	
	virtual	void			MessageReceived(BMessage *message);
	virtual	void			MouseDown(BPoint point);	
	virtual	void			AboutRequested();
	
	static 	BArchivable*	Instantiate(BMessage *msg);		   
	virtual	status_t		Archive(BMessage *msg, bool deep=true) const;
		
			void 			SetBackgroundColor(rgb_color color);
			
			void			_LoadDB();
			void			_OpenTakeNotes();
			void			_Quit();
			

	private:
	
		bool			fReplicated;
		bool			fInDeskbar;
		
		AppHashTable	*fHash;
		
		BFile			fDatabase;
		
		NoteText		*fNoteText;
		BMessenger 		*fMessenger;
		BString 		*message;
	

};

#endif