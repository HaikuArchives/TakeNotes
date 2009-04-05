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

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

// Costruttore
class NoteWindow : public BWindow {

	public:		
							NoteWindow(BRect frame);
			virtual void	MessageReceived(BMessage* message);
			virtual bool	QuitRequested();
	private:
		NoteView *fNoteView;
		ColorWindow * fColorWindow;
		BMenuBar *fNoteMenuBar;
		BMenu *fFontMenu;
		BMenu *fEditMenu;
		BMenu *fTagMenu;
		BWindow *fTagWindow;
};

#endif

