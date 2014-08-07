/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Stefano Celentano
 * 
 * Last revision: Stefano Celentano, 15th May 2009
 *
 * Description: AlarmView: it allows you to set an alarm
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

// Drawing the window
void AlarmView :: Draw (BRect updateRect) {
	MovePenTo(BPoint(20.0, 20.0));
	DrawString ("Insert time and date to set an alarm");
}


