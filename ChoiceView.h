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

#ifndef CHOICE_VIEW_H
#define CHOICE_VIEW_H

#include <View.h>
#include <Messenger.h>

class ChoiceView : public BView {
	
	public:
	
					 ChoiceView (BRect frame, char *name, BHandler *handler);
		virtual void AttachedToWindow ();
		virtual void Draw(BRect updateRect);
		virtual void MouseDown(BPoint point);
		virtual void MessageReceived(BMessage* );
		
	private:
		BMessenger	*fMessenger;
};

#endif