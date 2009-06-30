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
 * Description: Header of the ChoiceView
 */

#ifndef CHOICE_VIEW_H
#define CHOICE_VIEW_H

// Libraries
#include <Button.h>
#include <View.h>
#include <Messenger.h>

// Definition of the class
class ChoiceView : public BView {
	
	public:
	
					 ChoiceView (BRect frame, char *name, BHandler *handler);
		virtual void AttachedToWindow ();

		
	private:
	
		
		BMessenger	*fMessenger;
		BButton		*fOkayButton;
		BButton		*fUndoButton;
};

#endif
