/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 16h May 2009
 *
 * Description: TODO
 */
 
#include "ColorMenuItem.h"
#include <Message.h>

ColorMenuItem :: ColorMenuItem(const char* label, rgb_color color, BMessage *message) 
			  : BMenuItem (label, message, 0, 0), fColor(color){
			  
}

void ColorMenuItem :: DrawContent() {
	BMenu *menu = Menu();
	
	if (menu) {
		rgb_color menuC = menu -> HighColor();
		menu -> SetHighColor (fColor);
		BMenuItem :: DrawContent();
		menu -> SetHighColor (menuC);
	}
}