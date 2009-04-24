/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Stefano Celentano
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 20th April 2009
 *
 * Description: TODO
 */


#include "NoteWindow.h"

#include <Clipboard.h>
#include <Autolock.h>
#include <Application.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

// Costanti
#define MENU_CHANGE_COLOR 'mcg'
#define COLOR_CHANGED 'ccrq'
#define FONT_SIZE 'fnts'
#define FONT_COLOR 'fntc'
#define FONT_FAMILY 'fntf'
#define FONT_STYLE 'ftst'
#define TEXT_CHANGED 'txch'
#define ADD_DATA 'addd'
#define SET_ALARM 'salr'
#define ALARM_MSG 'alrm'

#define MENU_BAR_HEIGHT 18;
#define TEXT_INSET 10

// Strutture
const struct tm gettime() {
    time_t    t=time(NULL);
    return *localtime(&t);
}

// Costruttore
NoteWindow::NoteWindow(BRect frame)
	: BWindow (frame, "TakeNotes", B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS){
	
	//Variables
	BMessage	*message;
	
	BRect 		menuBarRect,
				frameView,
				frameText;
				
	BMenuItem 	*menuItem;
	
	BMenu		*sizeFont,
				*colorFont,
				*fontMenu;
	
	char		*label;
	
	font_family plainFamily,
	 			family;
	font_style 	plainStyle,
				style;
	
	uint32		flags;
	
	int32		numFamilies,
				numStyles,
				fontSizes[] = {9,10,11,12,14,18,24,36,48,72};

	rgb_color 	black = {0,0,0},
				red = {0,255,0},
				green = {0,255,0},
				blue = {0,0,255},
				yellow = {254,254,92};
	rgb_color   colors[] = {black, red, green, blue, yellow};
	
		
	// Struttura dati
	fDati.Id = 1;
	fDati.Titolo = "Titolo";

	
	
	//flag di undo
	
	fCanUndo=false;		//se non c'è testo non posso fare undo
	fUndoFlag=false;
	
	
	// Barra del Menu
	
	menuBarRect = Bounds();
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
	fSettingsMenu -> AddItem (fSetAlarmItem = new BMenuItem ("Set alarm",
			new BMessage (SET_ALARM)));					
	
	//Edit

	fEditMenu -> AddItem (fUndoItem = new BMenuItem("Can't Undo", new BMessage(B_UNDO), 'Z'));
	fUndoItem -> SetEnabled(false);		//senza un messaggio TEXT_CHANGED, non deve essere possibile fare undo
	
	fEditMenu -> AddSeparatorItem();
	
	fEditMenu -> AddItem (fCutItem = new BMenuItem("Cut", new BMessage(B_CUT), 'X'));
	fCutItem -> SetTarget(this);
	
	fEditMenu -> AddItem (fCopyItem = new BMenuItem("Copy", new BMessage(B_COPY), 'C'));
	fCopyItem -> SetTarget(this);
	
	fEditMenu -> AddItem (fPasteItem = new BMenuItem("Paste", new BMessage(B_PASTE), 'V'));
	fPasteItem -> SetTarget(this);
	
	fEditMenu -> AddItem (fSelectAllItem = new BMenuItem("Select All", new BMessage(B_SELECT_ALL), 'A'));
	fSelectAllItem -> SetTarget(this);
	
	// Font: Size
	
	sizeFont = new BMenu ("Size");
	sizeFont -> SetRadioMode (true);
	fFontMenu -> AddItem (sizeFont);
	
	for (uint32 i = 0; i < sizeof(fontSizes) / sizeof(fontSizes[0]); i++ ){
		message = new BMessage (FONT_SIZE);
		message -> AddFloat ("size", fontSizes[i]);
		
		char label[64];
		snprintf(label, sizeof(label), "%ld", fontSizes[i]);
		sizeFont -> AddItem (menuItem = new BMenuItem (label, message));
		
			if (i == 3)
				menuItem -> SetMarked(true);
	}
	
	// Font: Color
	colorFont = new BMenu ("Color");
	colorFont -> SetRadioMode (true);
	fFontMenu -> AddItem (colorFont);
	

	
	// Stampa del menu
	
	delete message;
	
	for (uint32 i = 0; i < sizeof(colors) / sizeof(colors[0]); i++ ){

		message = new BMessage (FONT_COLOR);
		message -> AddInt8 ("red", (int8)colors[i].red);
		message -> AddInt8 ("green", (int8)colors[i].green);
		message -> AddInt8 ("blue", (int8)colors[i].blue);
		
		label = NULL;
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
		colorFont -> AddItem (menuItem = new BMenuItem (label, message));
			// Marco l'item come utilizzato
			if (i == 0)
				menuItem -> SetMarked(true);
	}
	
	fFontMenu -> AddSeparatorItem();
	
	// Tipo di carattere
	
	fCurrentFont = 0;
	
	be_plain_font -> GetFamilyAndStyle (&plainFamily,&plainStyle);
	
	numFamilies = count_font_families();
	for (int32 i = 0; i < numFamilies; i++) {
		
			if (get_font_family(i,&family) == B_OK) {
				fontMenu = new BMenu (family);
				fontMenu -> SetRadioMode (true);	// Click esclusivo
				fFontMenu -> AddItem (menuItem = new BMenuItem (fontMenu,
					new BMessage (FONT_FAMILY)));
				if (!strcmp (plainFamily,family)) {
					menuItem -> SetMarked (true);
					fCurrentFont = menuItem;
				}
				numStyles = count_font_styles (family);
				for (int32 j = 0; j < numStyles; j++) {
				
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
	frameView = Bounds();
	
	frameView.top = fNoteMenuBar->Bounds().Height() + 1;
	frameView.right -= B_V_SCROLL_BAR_WIDTH;
	frameView.bottom -= B_H_SCROLL_BAR_HEIGHT;
	frameView.left = 0;
	
	
	frameText = frameView;
	
	frameText.OffsetTo(B_ORIGIN);
	frameText.InsetBy(TEXT_INSET,TEXT_INSET);
	
	fNoteView = new NoteView (frameView, frameText, "TakeNotes",this); // necessario per gli about
	fNoteView->SetDoesUndo(true);
	fNoteView->MakeFocus(); 
	fNoteView->SetStylable(true);
	
	// ScrollView
	fScrollView = new BScrollView("scrollview", fNoteView, B_FOLLOW_ALL, 0, true, true, B_NO_BORDER);
	
	// Associamolo alla Window
	AddChild(fNoteMenuBar);
	AddChild(fScrollView);

	Show();
	
}
	
// Funzione per il cambio di tipo di font
void NoteWindow :: SetFontStyle (const char* fontFamily, const char* fontStyle) {
	
	// Variables
	BMenuItem 	*superItem;
	BMenuItem	*menuItem;
	BFont 		font;
	
	font_family oldFamily;
	font_style 	oldStyle;
	
	uint32 		sameProperties;

	rgb_color 	sameColor;
	
	
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
	
	
	superItem = fFontMenu -> FindItem (fontFamily);
		if (superItem != NULL )
			superItem -> SetMarked (true);	// Marco quello selezionato
	menuItem = fFontMenu -> FindItem("Black");
	menuItem -> SetMarked(true);
}

// Funzione per la ricezione di messaggi
void NoteWindow :: MessageReceived(BMessage* message) {
	
	//Variables
		BFont	  font;
		BRect	  aRect;	
	
  const char 	  *fontFamily,
  				  *fontStyle;
  		char	  stringa[2];		  
  		void	  *ptr;		  
	
		int		  second,
				  minute,
				  hour,
				  day,
				  month,
				  year;
		uint32	  sameProperties;
		int8	  c;
		int16 	  i;
		float     fontSize;
	
		rgb_color colore,
			 	  sameColor;
			  
	
	switch (message -> what) {
		case MENU_CHANGE_COLOR:{
			
			aRect.Set(300,300,700,700);
			
			// Evito che ci siano due finestre di scelta colore			
			if (be_app->CountWindows() == 1)
				fColorWindow = new ColorWindow(aRect,this);
			}
			break;	
		// Colore di background
		case COLOR_CHANGED: {
				
			message->FindInt8("red", &c);
			colore.red = (uint8)c;
			message->FindInt8("green", &c);
			colore.green = (uint8)c;
			message->FindInt8("blue", &c);
			colore.blue = (uint8)c;
			fNoteView -> SetBackgroundColor(colore);
			
			// STRUTTURA DATI
			fDati.ColoreBack = colore;
			
		}
		break;	
		
		// Dimensione del font
		case FONT_SIZE: {
		
			if (message -> FindFloat ("size", &fontSize) == B_OK){
			
				fNoteView -> GetFontAndColor(&font, &sameProperties, &sameColor);
				font.SetSize(fontSize);
				fNoteView -> SetFontAndColor (&font, B_FONT_SIZE);
			}
		}
		break;
		
		// Colore del font
		case FONT_COLOR: {
		
			message->FindInt8("red", &c);
			colore.red = (uint8)c;
			message->FindInt8("green", &c);
			colore.green = (uint8)c;
			message->FindInt8("blue", &c);
			colore.blue = (uint8)c;
			
			fNoteView -> GetFontAndColor(&font, &sameProperties);
			fNoteView -> SetFontAndColor(&font,0,&colore);
		}
		break;
		
		// Tipo di font
		case FONT_FAMILY: {
			fontFamily = NULL;
			fontStyle = NULL;
			
			message -> FindPointer ("source", &ptr);
			fCurrentFont = static_cast <BMenuItem*>(ptr);
			fontFamily = fCurrentFont -> Label();
			SetFontStyle (fontFamily, fontStyle);
		}
		break;
		case FONT_STYLE: {
			fontFamily = NULL;
			fontStyle = NULL;
			
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
		
		//Messaggi per l'edit		
		case B_CUT:
			fNoteView->Cut(be_clipboard);
		break;
		
		case B_COPY:
			fNoteView->Copy(be_clipboard);
		break;
		
		case B_PASTE:
			fNoteView->Paste(be_clipboard);
		break;
		
		case B_SELECT_ALL:
			fNoteView->SelectAll();
		break;
		
		//Messaggio per il cambiamento del testo (serve per il can't undo)
		case TEXT_CHANGED:
			if (fUndoFlag) {
			
				fCanUndo = false;
				fCanRedo = true;
				fUndoItem -> SetLabel("Redo");
				fUndoItem -> SetEnabled(true);
				fUndoFlag = false;
			}
			
			else {
			
				fCanUndo = true;		
				fCanRedo = false;
				fUndoItem -> SetLabel("Undo");
				fUndoItem -> SetEnabled(true);
				fRedoFlag = false;
			
		    }
		    // Modifico la struttura dati
		    fDati.Contenuto = (char*)fNoteView -> Text();
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
			day = gettime().tm_mday;
			month = gettime().tm_mon + 1;
			year = gettime().tm_year+1900;
			second = gettime().tm_sec;
			min = gettime().tm_min;
			hour = gettime().tm_hour;
			
			
			sprintf(stringa, "%d/%d/%d - %d:%d:%d", day,
					month, year, hour, min, second);
			
			fNoteView -> MakeFocus();
			fNoteView -> Insert(stringa);
		}
		break;
		
		case SET_ALARM: {
			
			aRect.Set(300,300,800,600);
			fAlarmWindow = new AlarmWindow(aRect,this);
		}		
		break;
		
		// Setto i campi di data e ora scadenza
		case ALARM_MSG: {
			
			
			message -> FindInt16("year", &i);
			fDati.Anno = i;
			message -> FindInt16("month", &i);
			fDati.Mese = i;
			message -> FindInt16("day", &i);
			fDati.Giorno = i;
			message -> FindInt16("hour", &i);
			fDati.Ora = i;
			message -> FindInt16("minute", &i);
			fDati.Minuto = i;
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
