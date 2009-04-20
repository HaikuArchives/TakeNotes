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

/* == STRUTTURA DATI ==
 * Struttura contenente tutte le informazioni inerenti al post-it corrente
 * Informazioni contenute:
 * - ID - Per ora statico (da definire come impostarlo)
 * - Titolo - Per ora statico (si inserisce quando si trasforma in XML)
 * - Contenuto
 * - Font corrente
 * - Colore del font corrente
 * - Dimensione del carattere corrente
 * - Colore dello sfondo corrente
 * - Data di scadenza (giorno - mese - anno)
 * - Ora di scadenza (ora - minuto - secondo)
 * - = IN FUTURO: tags! =
 */
struct DatiPostIt {
	int Id;					// Id del post-it
	char* Titolo;			// Titolo del post-it
	char* Contenuto;		// Contenuto (testo) del post-it
	BFont Font;				// Font corrente DA DECIDERE
	rgb_color ColoreFont;	// Colore del font DA DECIDERE
	int DimFont;			// Dimensione del carattere DA DECIDERE
	rgb_color ColoreBack;	// Colore dello sfondo
	int Giorno;				// Giorno di scadenza
	int Mese;				// Mese di scadenza
	int Anno;				// Anno di scadenza
	int Ora;				// Ora di scadenza
	int Minuto;				// Minuto di scadenza
};

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
		
		// Struttura Dati
		DatiPostIt fDati;
};

#endif

