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
 * Description: View that allows you to change the background color
 */

// Libraries
#include "ColorView.h"
#include "StringView.h"

//translation 
#include <Catalog.h>
#include <TranslationUtils.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ColorWindow"
// Messages
#define COLOR_CHANGED 'ccrq'

// Constructor
ColorView :: ColorView(BRect rect, const char *name,BHandler *handler)
	   	  : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW){
}

// Font
void ColorView :: AttachedToWindow() {
	SetFont (be_bold_font);
	SetFontSize(12);
}

// Drawing the window
void ColorView :: Draw (BRect updateRect) {
	// Variables
	BStringView *instructions;
	BRect frame(10,20,600,35);
	const char *text = B_TRANSLATE("Change the color. It will be applied immediatly.");

	instructions = new BStringView (frame, "instructions", text);
	AddChild(instructions);
}

// When the user clicks...
void ColorView :: MouseDown(BPoint point) {

}
