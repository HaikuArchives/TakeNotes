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
 * Description: Header of the ColorWindow
 */


#ifndef COLOR_WINDOW_H
#define COLOR_WINDOW_H

#include <Window.h>
#include <ColorControl.h>


// Declaration of the class
class ColorWindow : public BWindow {

	public:

					 ColorWindow(BRect frame, BHandler *handler, rgb_color);
		virtual bool QuitRequested();
		virtual	void MessageReceived(BMessage*);
		virtual void Quit();
		const rgb_color gBgColor = {255,240,113};


	private:

		BColorControl 	*fColorControl;
		BMessenger		*fMessenger;
};

#endif
