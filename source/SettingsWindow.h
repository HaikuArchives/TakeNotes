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


#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include "ColorView.h"

#include <Window.h>


// Declaration of the class
class SettingsWindow : public BWindow {

	public:

					 SettingsWindow(BRect frame, BHandler *handler, rgb_color color);
		virtual bool QuitRequested();
		virtual	void MessageReceived(BMessage*);
		virtual void Quit();
		const rgb_color fBgColor = {255,240,113};

	private:

		BColorControl 	*fColorControl;
		ColorView 	 	*fColorView;
		BMessenger		*fMessenger;
};

#endif
