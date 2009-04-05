/*
 * Semplice finestra
 *
 * Autore: Ciceri Eleonora
 * Data: 14 Marzo 2009 14.15
 */

// Librerie
#ifndef COLOR_WINDOW_H
#define COLOR_WINDOW_H

// Window.h fa sempre parte di Application
#ifndef _WINDOW_H
#include <Window.h>
#endif

#ifndef COLOR_VIEW_H
#include "ColorView.h"
#endif

// Dichiarazione della classe
class ColorWindow : public BWindow {
	public:
		ColorWindow(BRect frame);
		virtual bool QuitRequested();
	private:
		ColorView *fColorView;
};

#endif