/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Eleonora Ciceri
 *			Ilio Catallo
 *
 * Last revision: Ilio Catallo, 10th June 2009
 *
 * Description: Header of the window.
 */


#ifndef NOTE_WINDOW_H
#define NOTE_WINDOW_H

// Out libraries
#include "NoteView.h"
#include "NoteText.h"
#include "ColorWindow.h"
#include "AlarmWindow.h"
#include "ChoiceWindow.h"
#include "TagsWindow.h"

// Other Libraries
#include <Window.h>
#include <ScrollView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <FilePanel.h>

/* == DATA STRUCTURE ==
 * Structure containing all the informations about the current post-it
 * Information that it contains:
 * - Content
 * - Application associated
 * - Alarm date (day - month - year)
 * - Alarm hour (hour - minute)
 * - = TODO: TAGS! =
 */
struct AlarmData {
	int Day;				// Alarm - Day
	int Month;				// Alarm - Month
	int Year;				// Alarm - Year
	int Hour;				// Alarm - Hour
	int Minute;				// Alarm - Minute
};

// Constructor
class NoteWindow : public BWindow {

	public:
								NoteWindow(int32 id);
								NoteWindow(entry_ref *ref);
								~NoteWindow();
			virtual void		MessageReceived(BMessage* message);
			virtual void		Quit();
			virtual bool		QuitRequested();
					void		AboutRequested();
					void		InitWindow();
					void		CreateOtherWindows();
					void		_LoadDB();
					void 		SetFontStyle (const char* fontFamily, const char *fontStyle);
					status_t	Save (BMessage*);
					status_t	_SaveDB(const char* signature);

	private:

		NoteView 		*fNoteView;
		NoteText		*fNoteText;
		ColorWindow 	*fColorWindow;
		AlarmWindow 	*fAlarmWindow;
		ChoiceWindow 	*fChoiceWindow;
		TagsWindow		*fTagsWindow;
		BMenuBar 		*fNoteMenuBar;
		BMenu			*fFileMenu;
		BMenu 			*fFontMenu;
		BMenu 			*fEditMenu;
		BMenu 			*fSettingsMenu;
		BMenu 			*fAboutMenu;
		BWindow 		*fTagWindow;
		BMenuItem 		*fCurrentFont;
		BScrollView		*fScrollView;

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
		BMenuItem	*fAddDateAndTimeItem;
		BMenuItem	*fSetTagsItem;
		BMenuItem	*fSetAppItem;
		BMenuItem 	*fSetAlarmItem;
		BMenuItem   *fLink;

		// Undo flags
		bool 		fUndoFlag;
		bool 		fCanUndo;
		bool 		fCanRedo;
		bool 		fRedoFlag;

		// Data structure
		AlarmData 	fData;

		//Messaging
		BMessage		*fSaveMessage;
		BMessenger 		fMessenger;
		BMessageRunner 	*runner;

		//Alarm flag
		bool alarm_set;

		// Save panel
		BFilePanel	*fSavePanel;
		
		// Open panel
		BFilePanel	*fOpenPanel;

		// Hash table
		BFile			fDatabase;
		AppHashTable	*fHash;

		void _CreateNoteView(void);
};

#endif
