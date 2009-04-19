/* Finestra principale
 * 
 * Autore: Eleonora Ciceri
 * Data: 2 Aprile 2009
 */

// Include
#ifndef NOTE_WINDOW_H
#define NOTE_WINDOW_H 

#ifndef _WINDOW_H
#include <Window.h>
#endif

#ifndef MY_DRAWING_H
#include "NoteView.h"
#endif

#ifndef COLOR_WINDOW_H
#include "ColorWindow.h"
#endif

#ifndef ALARM_WINDOW_H
#include "AlarmWindow.h"
#endif

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

#ifndef _SCROLLVIEW_H
#include <ScrollView.h>
#endif

// Costruttore
class NoteWindow : public BWindow {

	public:		
							NoteWindow(BRect frame);
			virtual void	MessageReceived(BMessage* message);
			virtual bool	QuitRequested();
			void SetFontStyle (const char* fontFamily, const char *fontStyle);
	
	private:
		NoteView *fNoteView;
		ColorWindow * fColorWindow;
		AlarmWindow * fAlarmWindow;
		BMenuBar *fNoteMenuBar;
		BMenu *fFontMenu;
		BMenu *fEditMenu;
		BMenu *fSettingsMenu;
		BMenu *fAboutMenu;
		BWindow *fTagWindow;
		BMenuItem *fCurrentFont;
		BScrollView	*fScrollView;
		
		//MenuItems (menu Edit)
		
		BMenuItem* fUndoItem;
		BMenuItem* fCutItem;
		BMenuItem* fCopyItem;
		BMenuItem* fPasteItem;
		BMenuItem* fSelectAllItem;
		
		//MenuItems (menu Settings)
		
		BMenuItem* fChangeBackgroundColorItem;
		BMenuItem* fSetAlarmItem;
		
		//flag di undo
		bool fUndoFlag;
		bool fCanUndo;
		bool fCanRedo;
		bool fRedoFlag;
};

#endif

