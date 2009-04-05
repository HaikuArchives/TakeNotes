/*
 * Semplice finestra
 *
 * Autore: Ciceri Eleonora
 * Data: 14 Marzo 2009 14.15
 */

// Librerie
#ifndef _APPLICATION_H  // Application è l'insieme delle API
						// di gestione degli eventi
						// tipo mouse clickato ecc
#include <Application.h>
#endif

#ifndef COLOR_WINDOW_H
#include "ColorWindow.h"
#endif

#ifndef COLOR_VIEW_H
#include "ColorView.h"
#endif

// Costruttore
// Me la crea tenendo la dimensione del rettangolo BRect
ColorWindow :: ColorWindow (BRect frame)
		: BWindow (frame, "Cambia il colore...", B_TITLED_WINDOW,B_NOT_RESIZABLE) {
	frame.OffsetTo(B_ORIGIN);
	fColorView = new ColorView (frame, "ColorView");
	AddChild(fColorView);
	
	Show();
}

// Funzione attivata quando chiudo la finestra
bool ColorWindow :: QuitRequested() {
	return (true);
}