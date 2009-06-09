/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Ilio Catallo, 9th June 2009
 *
 * Description: TODO
 */

#ifndef CHOICE_VIEW_H
#define CHOICE_VIEW_H

#include <View.h>
#include <Button.h>
#include <Messenger.h>

class ChoiceView : public BView {
	
	public:
	
					 ChoiceView (BRect frame, char *name, BHandler *handler);
		virtual void AttachedToWindow ();
		virtual void Draw(BRect updateRect);

		
	private:
	
		
		BMessenger	*fMessenger;
		BButton		*fOkayButton;
		BButton		*fUndoButton;
};

#endif