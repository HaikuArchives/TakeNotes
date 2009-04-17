/* Finestra principale
 *
 * Autore: Eleonora Ciceri, Ilio Catallo, Stefano Celentano
 * Data: 2 Aprile 2009
 * Ultima revisione: Ilio Catallo, 10 Aprile 2009
 */
 
// Costanti
#define MENU_CHANGE_COLOR 'mcg'
#define COLOR_CHANGED 'ccrq'
#define FONT_SIZE 'fnts'
#define FONT_COLOR 'fntc'
#define FONT_FAMILY 'fntf'
#define FONT_STYLE 'ftst'
#define TEXT_CHANGED 'txch'
#define ADD_DATA 'addd'


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

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef _CLIPBOARD_H
#include <Clipboard.h>
#endif

#ifndef _SCROLLVIEW_H
#include <ScrollView.h>
#endif

#include <Autolock.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MENU_BAR_HEIGHT 18;
#define TEXT_INSET 10

// Strutture
const struct tm gettime() {
    time_t    t=time(NULL);
    return *localtime(&t);
}

// Costruttore
NoteWindow::NoteWindow(BRect frame)
	: BWindow (frame,"TakeNotes",B_TITLED_WINDOW,B_NOT_RESIZABLE){
	
	//frame.OffsetTo(B_ORIGIN);
	
	
	//flag di undo
	
	fCanUndo=false;		//se non c'è testo non posso fare undo
	fUndoFlag=false;
	
	
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
	
	fFontMenu -> SetRadioMode(true);
	
	// Menu Item
	
	// Settings
	
	fSettingsMenu -> AddItem (fChangeBackgroundColorItem = new BMenuItem ("Change background color",
			new BMessage (MENU_CHANGE_COLOR)));
	fSettingsMenu -> AddItem (fChangeBackgroundColorItem = new BMenuItem ("Add date and time",
			new BMessage (ADD_DATA)));		
	
	//Edit

	fEditMenu -> AddItem (fUndoItem = new BMenuItem("Can't Undo", new BMessage(B_UNDO), 'Z'));
	fUndoItem -> SetEnabled(false);		//senza un messaggio TEXT_CHANGED, non deve essere possibile fare undo
	
	fEditMenu -> AddSeparatorItem();
	
	fEditMenu -> AddItem (fCutItem = new BMenuItem("Cut", new BMessage(B_CUT), 'X'));
	fCutItem -> SetTarget(NULL,this);
	
	fEditMenu -> AddItem (fCopyItem = new BMenuItem("Copy", new BMessage(B_COPY), 'C'));
	fCopyItem -> SetTarget(NULL,this);
	
	fEditMenu -> AddItem (fPasteItem = new BMenuItem("Paste", new BMessage(B_PASTE), 'V'));
	fPasteItem -> SetTarget(NULL,this);
	
	fEditMenu -> AddItem (fSelectAllItem = new BMenuItem("Select All", new BMessage(B_SELECT_ALL), 'A'));
	fSelectAllItem -> SetTarget(NULL,this);
	
	// Font: Size
	BMenuItem* menuItem;
	BMenu *sizeFont = new BMenu ("Size");
	sizeFont -> SetRadioMode (true);
	fFontMenu -> AddItem (sizeFont);
	int32 fontSizes[] = {9,10,11,12,14,18,24,36,48,72};
	for (uint32 i = 0; i < sizeof(fontSizes) / sizeof(fontSizes[0]); i++ ){
		BMessage *msg = new BMessage (FONT_SIZE);
		msg -> AddFloat ("size", fontSizes[i]);
		
		char label[64];
		snprintf(label, sizeof(label), "%ld", fontSizes[i]);
		sizeFont -> AddItem (menuItem = new BMenuItem (label, msg));
		
			if (i == 3)
				menuItem -> SetMarked(true);
	}
	
	// Font: Color
	BMenu *colorFont = new BMenu ("Color");
	colorFont -> SetRadioMode (true);
	fFontMenu -> AddItem (colorFont);
	
	rgb_color black = {0,0,0};
	rgb_color red = {255,0,0};
	rgb_color green = {0,255,0};
	rgb_color blue = {0,0,255};
	rgb_color yellow = {254,254,92};
	rgb_color colors[] = {black, red, green, blue, yellow};
	// Stampa del menu
	for (uint32 i = 0; i < sizeof(colors) / sizeof(colors[0]); i++ ){
		BMessage *msg = new BMessage (FONT_COLOR);
		msg -> AddInt8 ("red", (int8)colors[i].red);
		msg -> AddInt8 ("green", (int8)colors[i].green);
		msg -> AddInt8 ("blue", (int8)colors[i].blue);
		
		char *label;
		switch (i) {
			case 0:
				label = "Black";
				break;
			case 1:
				label = "Red";
				break;
			case 2:
				label = "Green";
				break;
			case 3:
				label = "Blue";
				break;
			case 4:
				label = "Yellow";
				break;
			default:
				break;
		}
		colorFont -> AddItem (menuItem = new BMenuItem (label, msg));
			// Marco l'item come utilizzato
			if (i == 0)
				menuItem -> SetMarked(true);
	}
	
	fFontMenu -> AddSeparatorItem();
	
	// Tipo di carattere
	font_family plainFamily;
	font_style plainStyle;
	fCurrentFont = 0;
	
	be_plain_font -> GetFamilyAndStyle (&plainFamily,&plainStyle);
	BMenu *fontMenu;
	int32 numFamilies = count_font_families();
	for (int32 i = 0; i < numFamilies; i++) {
		font_family family;
			if (get_font_family(i,&family) == B_OK) {
				fontMenu = new BMenu (family);
				fontMenu -> SetRadioMode (true);	// Click esclusivo
				fFontMenu -> AddItem (menuItem = new BMenuItem (fontMenu,
					new BMessage (FONT_FAMILY)));
				if (!strcmp (plainFamily,family)) {
					menuItem -> SetMarked (true);
					fCurrentFont = menuItem;
				}
				int32 numStyles = count_font_styles (family);
				for (int32 j = 0; j < numStyles; j++) {
					font_style style;
					uint32 flags;
					if (get_font_style(family,j,&style,&flags)==B_OK) {
						fontMenu -> AddItem (menuItem = new BMenuItem
							(style, new BMessage(FONT_STYLE)));
						if (!strcmp (plainStyle, style)) 
							menuItem -> SetMarked(true);
					}
				}
			}
	}
	
		
	
	// View principale	
	BRect frameView = Bounds();
	
	frameView.top = fNoteMenuBar->Bounds().Height() + 1;
	frameView.right -= B_V_SCROLL_BAR_WIDTH;
	frameView.left = 0;
	
	
	BRect frameText = frameView;
	
	frameText.OffsetTo(B_ORIGIN);
	frameText.InsetBy(TEXT_INSET,TEXT_INSET);
	
	fNoteView = new NoteView (frameView, frameText, "TakeNotes",this); // necessario per gli about
	fNoteView->SetDoesUndo(true);
	fNoteView->MakeFocus(); 
	fNoteView->SetStylable(true);
	
	// ScrollView
	
	fScrollView = new BScrollView("scrollview", fNoteView, B_FOLLOW_ALL, 0, false, true, B_NO_BORDER);
	
	// Associamolo alla Window
	

	
	AddChild(fNoteMenuBar);
	AddChild(fScrollView);

	Show();
	}
	
// Funzione per il cambio di tipo di font
void NoteWindow :: SetFontStyle (const char* fontFamily, const char* fontStyle) {
	// Variabili
	BFont font;
	uint32 sameProperties;
	font_family oldFamily;
	font_style oldStyle;
	rgb_color sameColor;
	
	fNoteView -> GetFontAndColor (&font, &sameProperties, &sameColor);
	font.GetFamilyAndStyle (&oldFamily, &oldStyle); // raccolgo famiglia e
													// stile attuali
	if (strcmp (oldFamily, fontFamily)) {// Se richeisto il font in uso
		BMenuItem *oldItem = fFontMenu -> FindItem (oldFamily);
		if (oldItem != NULL)
			oldItem -> SetMarked (false);	// Gli tolgo la marcatura
	}		
	font.SetFamilyAndStyle (fontFamily, fontStyle);
	fNoteView -> SetFontAndColor (&font);
	BMenuItem *superItem;
	superItem = fFontMenu -> FindItem (fontFamily);
		if (superItem != NULL )
			superItem -> SetMarked (true);	// Marco quello selezionato
	BMenuItem *menuItem = fFontMenu -> FindItem("Black");
	menuItem -> SetMarked(true);
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
		// Colore di background
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
		
		// Dimensione del font
		case FONT_SIZE: {
			float fontSize;
			
			if (message -> FindFloat ("size", &fontSize) == B_OK){
				uint32 sameProperties;
				BFont font;
				rgb_color sameColor;
				
				fNoteView -> GetFontAndColor(&font, &sameProperties, &sameColor);
				font.SetSize(fontSize);
				fNoteView -> SetFontAndColor (&font, B_FONT_SIZE);
			}
		}
		break;
		
		// Colore del font
		case FONT_COLOR: {
			rgb_color colore;
			int8 c;
			
			message->FindInt8("red", &c);
			colore.red = (uint8)c;
			message->FindInt8("green", &c);
			colore.green = (uint8)c;
			message->FindInt8("blue", &c);
			colore.blue = (uint8)c;
			uint32 sameProperties;
			BFont font;
			fNoteView -> GetFontAndColor(&font, &sameProperties);
			fNoteView -> SetFontAndColor(&font,0,&colore);
		}
		break;
		
		// Tipo di font
		case FONT_FAMILY: {
			const char* fontFamily = NULL;
			const char* fontStyle = NULL;
			void* ptr;
			
			message -> FindPointer ("source", &ptr);
			fCurrentFont = static_cast <BMenuItem*>(ptr);
			fontFamily = fCurrentFont -> Label();
			SetFontStyle (fontFamily, fontStyle);
		}
		break;
		case FONT_STYLE: {
			const char* fontFamily = NULL;
			const char* fontStyle = NULL;
			void* ptr;
			
			message -> FindPointer ("source", &ptr);
			BMenuItem *item = static_cast <BMenuItem*>(ptr);
			fontStyle = item -> Label();
			BMenu *menu = item -> Menu();
			if (menu != NULL) {
				fCurrentFont = menu -> Superitem();
				if (fCurrentFont != NULL)
					fontFamily = fCurrentFont -> Label();
			}			
			SetFontStyle (fontFamily, fontStyle);
		}
		break;
		
		//
		case TEXT_CHANGED:
			if (fUndoFlag) {
				fCanUndo = false;
				fCanRedo = true;
				fUndoItem -> SetLabel("Redo");
				fUndoItem -> SetEnabled(true);
				fUndoFlag = false;
			}
			
			else {
			
			fCanUndo = true;		//posso fare undo
			fCanRedo = false;
			fUndoItem -> SetLabel("Undo");
			fUndoItem -> SetEnabled(true);
			fRedoFlag = false;
			
		    }
		break;
				
		//Messaggio per la funzione di undo
		
		case B_UNDO:		//se ho ricevuto un B_UNDO message...
		if (fCanUndo)	//...e sono in uno stato in cui posso fare undo
			fUndoFlag = true;
		if (fCanRedo)	//...e sono in uno stato in cui posso fare redo
			fRedoFlag = true;
			
		fNoteView->Undo(be_clipboard);
		break;
		
		case ADD_DATA: {
			int day = gettime().tm_mday;
			int month = gettime().tm_mon + 1;
			int year = gettime().tm_year+1900;
			int second = gettime().tm_sec;
			int min = gettime().tm_min;
			int hour = gettime().tm_hour;
			
			char stringa[2];
			sprintf(stringa, "%d/%d/%d - %d:%d:%d", day,
					month, year, hour, min, second);
			
			fNoteView -> MakeFocus();
			fNoteView -> Insert(stringa);
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
