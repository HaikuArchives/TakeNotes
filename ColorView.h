/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 23th June 2009
 *
 * Description: TODO
 */

#ifndef COLOR_VIEW_H
#define COLOR_VIEW_H

#include <View.h>
#include <ColorControl.h>
#include <Messenger.h>

class ColorView : public BView {
	
	public:
	
					 ColorView (BRect frame, char *name, BHandler *handler);
		virtual void AttachedToWindow ();
		virtual void Draw(BRect updateRect);
		virtual void MouseDown(BPoint point);
};

#endif
