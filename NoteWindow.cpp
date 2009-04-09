/* Finestra principale
 *
 * Autore: Eleonora Ciceri, Ilio Catallo, Stefano Celentano
 * Data: 2 Aprile 2009
 * Ultima revisione: Ilio Catallo, 9 Aprile 2009
 */
 
// Costanti
#define MENU_CHANGE_COLOR 'mcg'
#define COLOR_CHANGED 'ccrq'


// Dichiarazione degli include
#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef NOTE_WINDOW_H
#include "NoteWindow.h"
#endif

#ifndef NOTE_VIEW_H
#include "NoteView.h"
#endif

#ifndef COLOR_WINDOW_H
#include "ColorWindow.h"
#endif

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

#ifndef _SCROLLVIEW_H
#include <ScrollView.h>
#endif

#define MENU_BAR_HEIGHT 18;
#define TEXT_INSET 10

// Costruttore
NoteWindow::NoteWindow(BRect frame)
	: BWindow (frame,"TakeNotes",B_TITLED_WINDOW,B_NOT_RESIZABLE){
	
	//frame.OffsetTo(B_ORIGIN);
	
	// Barra del Menu
	
	BRect menuBarRect = Bounds();
	menuBarRect.bottom = MENU_BAR_HEIGHT;
	fNoteMenuBar = new BMenuBar(menuBarRect,"Barra del menu");
	
	
	// Menu
	
	fFontMenu = new BMenu("Font");
	fEditMenu = new BMenu ("Edit");
	fSettingsMenu = new BMenu ("Settings");
	fAboutMenu = new BMenu("About");
	
	fNoteMenuBar -> AddItem (fEditMenu);
	fNoteMenuBar -> AddItem (fFontMenu);
	fNoteMenuBar -> AddItem (fSettingsMenu);
	fNoteMenuBar -> AddItem (fAboutMenu);
	
	// Menu Item
	
	// Settings
	
	BMenuItem* menuItem;
	fSettingsMenu -> AddItem (menuItem = new BMenuItem ("Change background color",
			new BMessage (MENU_CHANGE_COLOR)));
	
	//Edit
	
	fEditMenu -> AddItem (menuItem = new BMenuItem("Cut", new BMessage(B_CUT), 'X'));
	menuItem -> SetTarget(NULL,this);
	
	fEditMenu -> AddItem (menuItem = new BMenuItem("Copy", new BMessage(B_COPY), 'C'));
	menuItem -> SetTarget(NULL,this);
	
	fEditMenu -> AddItem (menuItem = new BMenuItem("Paste", new BMessage(B_PASTE), 'V'));
	menuItem -> SetTarget(NULL,this);
	
	fEditMenu -> AddItem (menuItem = new BMenuItem("Select All", new BMessage(B_SELECT_ALL), 'A'));
	menuItem -> SetTarget(NULL,this);
	
	// View principale

	
	BRect frameView = Bounds();
	
	frameView.top = fNoteMenuBar->Bounds().Height() + 5;
	frameView.right -= B_V_SCROLL_BAR_WIDTH;
	frameView.left = 0;
	
	BRect frameText = frameView;
	
	frameText.OffsetTo(B_ORIGIN);
	frameText.InsetBy(TEXT_INSET,TEXT_INSET);
	
	fNoteView = new NoteView (frameView, frameText, "NoteView");
	fNoteView->SetStylable(true);

	
	// Scrollbar
	
	fScrollView = new BScrollView("scrollview", fNoteView, B_FOLLOW_ALL, 0, false, true, B_NO_BORDER);
	
	// Associamolo alla Window
	AddChild(fScrollView);
	AddChild(fNoteMenuBar);
	
	
	fNoteView->MakeFocus(); 
	
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
				fColorWindow = new ColorWindow(aRect,this);
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
			fNoteView -> SetBackgroundColor(colore);
			
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
