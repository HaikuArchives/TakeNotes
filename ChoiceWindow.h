/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 30th May 2009
 *
 * Description: TODO
 */


#ifndef CHOICE_WINDOW_H
#define CHOICE_WINDOW_H

#include "ChoiceView.h"

#include <Window.h>
#include <Messenger.h>


// Declaration of the class
class ChoiceWindow : public BWindow {
	
	public:
		
					 ChoiceWindow(BRect frame, BHandler *handler);
		virtual bool QuitRequested();
		virtual void MessageReceived(BMessage*);
	
	private:
	
		ChoiceView 	 	*fChoiceView;
		BMessenger		*fMessenger;
};

#endif
