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
#include <Catalog.h>
#include <TranslationUtils.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "AlarmView"

// Constructor
AlarmView::AlarmView(BRect rect, const char *name)
		 : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW) {

}

void AlarmView :: AttachedToWindow() {

}

// Drawing the window
void AlarmView :: Draw (BRect updateRect) {

}


