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
 

#include "AlarmView.h"

// Constructor
AlarmView::AlarmView(BRect rect, char *name) 
		 : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW) {

}

void AlarmView :: AttachedToWindow() {
	SetFont (be_bold_font);
	SetFontSize(12);
}

void AlarmView :: Draw (BRect updateRect) {
	MovePenTo(BPoint(20.0, 20.0));
	DrawString ("Insert date and time to set an alarm");
}

