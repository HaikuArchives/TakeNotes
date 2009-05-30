/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Eleonora Ciceri
 *			Ilio Catallo
 * 
 * Last revision: Eleonora Ciceri, 30th May 2009
 *
 * Description: TODO
 */


#ifndef NOTE_WINDOW_H
#define NOTE_WINDOW_H 

#include "NoteView.h"
#include "NoteText.h"
#include "ColorWindow.h"
#include "AlarmWindow.h"

#include <Window.h>
#include <ScrollView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>

/* == DATA STRUCTURE ==
 * Structure containing all the informations about the current post-it
 * Information that it contains:
 * - ID - Now we're using it statically; =DEFINE HOW TO CHANGE IT=
 * - Title - Now we're using it statically =INSERT IT WHEN THE POST-IT IS DETACHED=
 * - Content
 * - Current Font
 * - Current Font color
 * - Current Font size
 * - Current background color
 * - Alarm date (day - month - year)
 * - Alarm hour (hour - minute)
 * - = TODO: TAGS! =
 */
struct DatiPostIt {
	int Id;					// Post-it id
	char* Titolo;			// Post-it title
	char* Contenuto;		// Post-it content
	BFont Font;				// Current font TODO
	rgb_color ColoreFont;	// Font color TODO
	int DimFont;			// Font size TODO
	rgb_color ColoreBack;	// Background color
	int Giorno;				// Alarm - Day
	int Mese;				// Alarm - Month
	int Anno;				// Alarm - Year
	int Ora;				// Alarm - Hour
	int Minuto;				// Alarm - Minute
};

// Constructor
class NoteWindow : public BWindow {

	public:		
								NoteWindow(int32 id);
								NoteWindow(entry_ref *ref);
			virtual void		MessageReceived(BMessage* message);
			virtual void		Quit();
			virtual bool		QuitRequested();
				
					void		InitWindow();
					void 		SetFontStyle (const char* fontFamily, const char *fontStyle);
					status_t	Save (BMessage*);
	
	private:
		NoteView 	*fNoteView;
		NoteText	*fNoteText;
		ColorWindow *fColorWindow;
		AlarmWindow *fAlarmWindow;
		BMenuBar 	*fNoteMenuBar;
		BMenu		*fFileMenu;
		BMenu 		*fFontMenu;
		BMenu 		*fEditMenu;
		BMenu 		*fSettingsMenu;
		BMenu 		*fAboutMenu;
		BWindow 	*fTagWindow;
		BMenuItem 	*fCurrentFont;
		BScrollView	*fScrollView;
		
		// Menu Items: file menu
		BMenuItem	*fSaveItem;
		BMenuItem	*fQuitItem;
		
		// MenuItems (Edit menu)
		
		BMenuItem 	*fUndoItem;
		BMenuItem 	*fCutItem;
		BMenuItem 	*fCopyItem;
		BMenuItem 	*fPasteItem;
		BMenuItem 	*fSelectAllItem;
		
		// MenuItems (Settings menu)
		
		BMenuItem 	*fChangeBackgroundColorItem;
		BMenuItem 	*fSetAlarmItem;
		BMenuItem   *fLink;
		
		// Undo flags
		bool 		fUndoFlag;
		bool 		fCanUndo;
		bool 		fCanRedo;
		bool 		fRedoFlag;
		
		// Data structure
		DatiPostIt 	fDati;
		
		//Messaging
		BMessenger fMessenger;
		BMessageRunner *runner;
		
		//Alarm flag
		bool alarm_set;
		
		// Save panel
		BFilePanel	*fSavePanel;

};

#endif

