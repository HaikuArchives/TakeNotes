/* Finestra principale
 *
 * Autore: Eleonora Ciceri, Ilio Catallo
 * Data: 2 Aprile 2009
 * Ultima revisione: Ilio Catallo, 5 Aprile 2009
 */
 
// Costanti
#define MENU_CHANGE_COLOR 'mcg'
#define COLOR_CHANGED 'CC'


// Dichiarazione degli include
#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef NOTE_WINDOW_H
#include "NoteWindow.h"
#endif

#ifndef MY_DRAWING_VIEW_H
#include "MyDrawingView.h"
#endif

#ifndef COLOR_WINDOW_H
#include "ColorWindow.h"
#endif

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

// Costruttore
NoteWindow::NoteWindow(BRect frame)
	: BWindow (frame,"TakeNotes",B_TITLED_WINDOW,B_NOT_RESIZABLE){
	frame.OffsetTo(B_ORIGIN);
	
	// Barra del Menu
	BRect menuBarRect;
	menuBarRect.Set(0,0,0,0);
	fNoteMenuBar = new BMenuBar(menuBarRect,"Barra del menu");
	AddChild(fNoteMenuBar);
	
	// Menu
	fFontMenu = new BMenu("Font");
	fNoteMenuBar -> AddItem (fFontMenu);
	fEditMenu = new BMenu ("Edit");
	fNoteMenuBar -> AddItem (fEditMenu);
	fTagMenu = new BMenu ("Tag");
	fNoteMenuBar -> AddItem (fTagMenu);
	
	// Menu Item
	BMenuItem* menuItem;
	fFontMenu -> AddItem (menuItem = new BMenuItem ("Change color...",
			new BMessage (MENU_CHANGE_COLOR)));
	fNoteView = new MyDrawingView (frame, "NoteView");
	AddChild(fNoteView);
	
	Show();
	}

// Funzione per la ricezione di messaggi
void NoteWindow :: MessageReceived(BMessage* message) {
	switch (message -> what) {
		case MENU_CHANGE_COLOR:{
			BRect aRect;
			aRect.Set(300,300,700,700);
			
			// Evito che ci siano due finestre di scelta colore			
			if (be_app->CountWindows() == 1)
				fColorWindow = new ColorWindow(aRect);
			}
			break;	
		case COLOR_CHANGED: {
			rgb_color colore;
			int8 c;
			
			message->FindInt8("red", &c);
			colore.red = (uint8)c;
			message->FindInt8("green", &c);
			colore.green = (uint8)c;
			message->FindInt8("blue", &c);
			colore.blue = (uint8)c;
			fNoteView -> SetViewColor(colore);
			fNoteView -> Invalidate();
		}
		break;		
			
		default:
			BWindow::MessageReceived(message);
	}
}
	
// Metodo per la chiusura della finestra
bool NoteWindow::QuitRequested(){

	be_app->PostMessage(B_QUIT_REQUESTED);
	return(true);
}
