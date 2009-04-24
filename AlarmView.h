/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Stefano Celentano
 * 
 * Last revision: Stefano Celentano, 19th April 2009
 *
 * Description: TODO
 */


#ifndef ALARM_VIEW_H
#define ALARM_VIEW_H

#include <View.h>

class AlarmView : public BView {
	public:
		AlarmView (BRect frame, char *name);
		virtual void AttachedToWindow ();
		virtual void Draw(BRect updateRect);
};

#endif