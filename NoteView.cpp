/*
 * View di disegno
 *
 * Autore: Eleonora Ciceri, Stefano Celentano
 * Data: 2 Aprile 2009
 * Ultima revisione: Ilio Catallo, 9 Aprile 2009
 */


#ifndef NOTE_VIEW_H
#include "NoteView.h"
#endif

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef _DRAGGER_H
#include <Dragger.h>
#endif

#ifndef _SCROLLBAR_H
#include <ScrollBar.h>
#endif

#include <stdio.h>
#include <stdlib.h>

// Messaggi
#define TEXT_CHANGED 'txch'
#define FONT_BOLD 'fntb'

// Costruttore
NoteView :: NoteView(BRect frame,BRect frameText, char *name, BHandler *handler )
	   	   : BTextView(frame, name, frameText, B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW){
	   	   
	   	   this->handler = handler;
	   	   fMessenger = new BMessenger(handler);
		   Init();	   	   
}

NoteView :: NoteView (BMessage *msg)
		   : BTextView(msg) {

			// va letto con calma 

			//BScrollBar *verticalBar;
//			BRect scrollFrame;
//			float min = 1.0;
//			float max = 1.0;
//			
//			scrollFrame = Bounds();
//			scrollFrame.left = scrollFrame.right;
//			scrollFrame.right = scrollFrame.left + 30;
//			
//			verticalBar = new BScrollBar(scrollFrame, "Vertical Bar",this,min,max, B_VERTICAL);
//			AddChild(verticalBar);
			
			Init();

}

NoteView :: ~NoteView(){}


void NoteView :: Init(){

		BDragger *dragger;	
		
	
		dragger = new BDragger(BRect(0,270,7,277),this,0);

		SetViewColor(254,254,92,255);
		AddChild(dragger);
}
		
		
status_t NoteView :: Archive (BMessage *msg,bool deep) const{
		
		status_t result = BTextView ::Archive(msg,deep);
		
		if (result == B_OK){
		
			result = msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
			if (result == B_OK) result = msg->AddString("class","NoteView");
		
		}
		
		return result;

}

BArchivable* NoteView :: Instantiate(BMessage *msg){

		if (!validate_instantiation(msg,"NoteView")) return NULL;
		return new NoteView(msg);
		
}

void NoteView :: AboutRequested(){

		BAlert *alert = new BAlert("TakeNotes","A program to take notes","OK");
		alert->SetShortcut(0,B_ESCAPE);
		alert->Go();

}


void NoteView :: MessageReceived(BMessage *msg){

		switch(msg->what){
		
			case B_ABOUT_REQUESTED:
				AboutRequested();
				break;
			default:
				BView::MessageReceived(msg);
		}
}

void NoteView :: SetBackgroundColor (rgb_color colore) {

	SetViewColor(colore);
	Invalidate();
		
}


void NoteView :: InsertText(const char* text, int32 length, int32 offset, const text_run_array *runs) {

    BMessage *msg = new BMessage (TEXT_CHANGED);
	fMessenger->SendMessage(msg);	
	BTextView::InsertText(text,length,offset,runs);

}