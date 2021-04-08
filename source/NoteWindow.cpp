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
 * Last revision: Ilio Catallo, 27th June 2009
 *
 * Description: Main window. In this file all the function of the window are implemented.
 */

// Our Libraries
#include "NoteApplication.h"
#include "NoteWindow.h"
#include "ColorMenuItem.h"

// Other Libraries
#include <Alert.h>
#include <Notification.h>

#include <Application.h>
#include <Autolock.h>
#include <Clipboard.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <Message.h>
#include <MessageRunner.h>
#include <NodeInfo.h>
#include <Path.h>
#include <Roster.h>
#include <FilePanel.h>
#include <Catalog.h>
#include <TranslationUtils.h>
#include <Url.h>

// System libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "NoteWindow"

// Messages
#define SET_COLOR 	'mcg'
#define COLOR_CHANGED 		'ccrq'
#define FONT_SIZE 			'fnts'
#define FONT_COLOR 			'fntc'
#define FONT_FAMILY 		'fntf'
#define FONT_STYLE 			'ftst'
#define TEXT_CHANGED 		'txch'
#define ADD_DATA 			'addd'
#define ADD_URL 			'addu'
#define SET_ALARM 			'salr'
#define SET_TAGS			'stag'
#define SET_APP				'stap'
#define ALARM_MSG 			'alrm'
#define GO_TO_LINK 			'gtlk'
#define ABOUT 				'bout'
#define CHECK_ALARM 		'chal'
#define NEW				'new'
#define OPEN				'open'
#define DELETE				'open'
#define SAVE				'save'
#define SAVE_AS 			'svas'
#define QUIT_APPL 			'qtpp'
#define CHOOSE_APPL 		'cspp'
#define RADIO_CHECKED 		'rdck'
#define COLOR_CLOSE			'_clc'
#define	TAGS_CLOSE			'_tgc'
#define	ALARM_CLOSE			'_alc'
#define	CHOICE_CLOSE		'_chc'
#define NOTE_SETTINGS		'ntst'
#define SETTINGS_CLOSE '_sec'
#define SET_DEFAULT_COLOR		'stdc'
#define LOAD_LAST_NOTE		'llno'
#define LIVE_IN_DESKBAR		'lind'

// Constants
#define MENU_BAR_HEIGHT 18
#define TEXT_INSET 10

using namespace std;

// Structures
const struct tm gettime() {
    time_t    t=time(NULL);
    return *localtime(&t);
}

// Constructor
NoteWindow::NoteWindow()
	:
	BWindow(BRect(100, 100, 350, 350), B_TRANSLATE("Untitled"), B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,
		B_ASYNCHRONOUS_CONTROLS),
	fRef(NULL)
{

	// Init the windows to create the menu
	InitWindow();

	_CreateNoteView();

	// It will be associated to the window
	AddChild(fNoteView);

	// Create the save file panel
	fSavePanel = new BFilePanel(B_SAVE_PANEL, new BMessenger(this), NULL, 0,
		false);

	Show();
}

// Construtor
NoteWindow :: NoteWindow(entry_ref *ref)
	:
	BWindow(BRect(100, 100, 350, 350), B_TRANSLATE("Untitled note"), B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS),
	fRef(new entry_ref(*ref))
{

	// Variables
	BMessage 	*msg = new BMessage;
	BFile		f;
	status_t 	result;
	BAlert 		*alert;
	BEntry		entry(ref, true);	// entry of possibly linked file
	BRect		viewRect;
	BString filename = B_TRANSLATE("Untitled note (filename)");
	char 		name[B_FILE_NAME_LENGTH] = "Untitled1";
	bool		isNoteFile = false;	// readable takenotes file
	bool		isNewFile = false;


	// Initialize all the "static" elements like MenuBar,Menu,MenuItems and so on
	InitWindow();

	// Check if the file exists and if it is a loadable takenotes file
	if (entry.InitCheck() == B_OK)
		if (entry.Exists()) {
			BNode node(&entry);
			if (node.InitCheck() == B_OK) {
				char type[B_MIME_TYPE_LENGTH];
				BNodeInfo nodeInfo(&node);
				if (nodeInfo.InitCheck() == B_OK &&
					nodeInfo.GetType(type) == B_OK &&
					strcmp(type, "application/takenotes") == 0 &&
					f.SetTo(ref, B_READ_ONLY) == B_OK)
					isNoteFile = true;
			}
		} else
			isNewFile = true;

	if (isNoteFile || isNewFile) {
		// Set the message that will store the path of the current note on FS
		fSaveMessage = new BMessage(B_SAVE_REQUESTED);

		if (fSaveMessage){
			// Add the required information to the fSaveMessage: the parent directory and the name of the file
			// Variables used ONLY in this case
			BEntry 		entry(ref, true); 			// Create a BEntry object of the note
			BEntry 		parent;			 			// Initialization of a BEntry object for the parent directory
			entry_ref 	parentRef;
			char 		name[B_FILE_NAME_LENGTH];

			entry.GetParent(&parent);		// We fill the BEntry parent object
			entry.GetName(name);			// We obtain the name of the note
			parent.GetRef(&parentRef);		// We fill the entry_ref parentRef struct

			// Actually add the necessary informations
			fSaveMessage->AddRef("directory", &parentRef);
			fSaveMessage->AddString("name", name);
		}

		entry.GetName(name);
	} else {
		alert = new BAlert("error", "Can't open the file!", "OK");
		alert->Go();
	}

	// Set the title of the window
	SetTitle(name);

	// Create the view from the flatten message stored in the FS
	if (isNoteFile) {
		// Fetch the view from the file
		msg->Unflatten(&f);

//		msg->PrintToStream();
		// Restore all the references for the NoteView's children
		fNoteView = new NoteView(msg);
		fScrollView = (BScrollView*) fNoteView->ChildAt(1);

		// We use NoteView::Archive both for replicant,deskbar and save/load
		// so we neeed to say clearly these sort of things :D
		fNoteView->SetReplicated(false);

		fNoteText = dynamic_cast<NoteText*>(fScrollView->Target());
		if (fNoteText != NULL) {
			fNoteText->SetReplicated(false);
			fNoteText->SetHandler(this);
		}

		// Resize the window to fit the real view size
		viewRect = fNoteView->Bounds();
		ResizeTo(viewRect.Width(),
			viewRect.Height() + fNoteMenuBar->Bounds().Height() + 1);
	} else
		_CreateNoteView();

	// Create the save file panel
	fSavePanel = new BFilePanel(B_SAVE_PANEL, new BMessenger(this), NULL, 0,
		false);

	// Add the view as a child and show the window
	AddChild(fNoteView);
	Show();
}

// Destructor
NoteWindow :: ~NoteWindow(){
	delete fRef;
	delete fSavePanel;
}

// Initializing the window
void NoteWindow :: InitWindow(){
//	SetSizeLimits(250, 10000, 250, 10000);

	// Initialize the messenger: the handler is the window itself
	fMessenger = BMessenger(this);

	// Variables
	BMessage	*message;
	BMessage	*check_alarm_msg;
	bigtime_t 	interval = 10000;
	BRect 		menuBarRect;
	BMenuItem 	*menuItem = NULL;
	BMenu		*sizeFont,
				*colorFont,
				*fontMenu;
//	 char		*label;
	BString label;
	font_family plainFamily,
	 			family;
	font_style 	plainStyle,
				style;
	uint32		flags;
	int32		numFamilies,
				numStyles,
				fontSizes[] = {9,10,11,12,14,18,24,36,48,72};
	rgb_color 	black = {0,0,0},
				red = {255,0,0},
				green = {0,255,0},
				blue = {0,0,255},
				yellow = {254,254,92},
				color;
	rgb_color   colors[] = {black, red, green, blue, yellow};

	//Initialize the fSaveMessage and fSettingsMessage
	fSaveMessage = NULL;

	// Create an empty message
	check_alarm_msg = new BMessage(CHECK_ALARM);

	// Initialize the message runner
	runner = new BMessageRunner(fMessenger, check_alarm_msg, interval);

	//Initialize the alarm flag to false (no one has set any alarm)
	alarm_set = false;

	// Undo flags
	fCanUndo 	= false;		// If there's no text I can't do undo
	fUndoFlag 	= false;


	// Menu bar
	menuBarRect = Bounds();
	menuBarRect.bottom = MENU_BAR_HEIGHT;
	fNoteMenuBar = new BMenuBar(menuBarRect,"Barra del menu");

	// Menu
	fFileMenu 		= new BMenu(B_TRANSLATE("Note"));
	fFontMenu 		= new BMenu(B_TRANSLATE("Font"));
	fEditMenu 		= new BMenu(B_TRANSLATE("Edit"));
	fSettingsMenu 	= new BMenu(B_TRANSLATE("Settings"));

	fNoteMenuBar->AddItem (fFileMenu);
	fNoteMenuBar->AddItem (fEditMenu);
	fNoteMenuBar->AddItem (fFontMenu);
	fNoteMenuBar->AddItem (fSettingsMenu);
	fNoteMenuBar->AddItem(new BMenuItem(B_TRANSLATE("About" B_UTF8_ELLIPSIS), new BMessage(B_ABOUT_REQUESTED)));

	fFontMenu->SetRadioMode(true);

	/*************** Menu Item ***************/

	// File menu
	fFileMenu->AddItem (fNewItem = new BMenuItem(B_TRANSLATE("New" B_UTF8_ELLIPSIS), new BMessage(NEW), 'N'));
	fFileMenu->AddItem (fOpenItem = new BMenuItem(B_TRANSLATE("Open" B_UTF8_ELLIPSIS), new BMessage(OPEN), 'O'));
	fNewItem->SetTarget(note_app);
	fOpenItem->SetTarget(note_app);

	fFileMenu->AddSeparatorItem();
	fFileMenu->AddItem (fSaveItem = new BMenuItem(B_TRANSLATE("Save"), new BMessage(SAVE), 'S'));
	fFileMenu->AddItem (fSaveItem = new BMenuItem(B_TRANSLATE("Save as" B_UTF8_ELLIPSIS), new BMessage(SAVE_AS), 'S', B_SHIFT_KEY));
	fFileMenu->AddSeparatorItem();
	fFileMenu->AddItem (fQuitItem = new BMenuItem (B_TRANSLATE("Quit"), new BMessage (QUIT_APPL), 'Q'));

	// Settings	menu
	//: rename it at the end to "Add..."
	fSettingsMenu->AddItem (fChangeBackgroundColorItem 	= new BMenuItem (B_TRANSLATE("Change background color"),		new BMessage (SET_COLOR)));
	fSettingsMenu->AddItem (fAddDateAndTimeItem 			= new BMenuItem (B_TRANSLATE("Insert date and time"),			new BMessage (ADD_DATA)));
	fSettingsMenu->AddItem (fSetAlarmItem 				= new BMenuItem (B_TRANSLATE("Set alarm"), 					new BMessage (SET_ALARM), 'L'));
	fSettingsMenu->AddItem (fSetTagsItem 					= new BMenuItem (B_TRANSLATE("Set tags"),					new BMessage (SET_TAGS)));
	fSettingsMenu->AddItem (fSetAppItem 					= new BMenuItem (B_TRANSLATE("Set preferred application"),	new BMessage (SET_APP)));
	fSettingsMenu->AddItem (fLink 						= new BMenuItem (B_TRANSLATE("Open the selected link"), 	new BMessage (GO_TO_LINK)));
	fSettingsMenu->AddItem (fLoadLastNote				= new BMenuItem (B_TRANSLATE("Load last note on startup"), 	new BMessage (LOAD_LAST_NOTE)));
	fSettingsMenu->AddItem (fLiveInDeskbar				= new BMenuItem (B_TRANSLATE("Use Deskbar Replicant"), 	new BMessage (LIVE_IN_DESKBAR)));
	fLoadLastNote->SetMarked(note_app->fSettingsMessage->FindBool("load_last_note"));
	fLiveInDeskbar->SetMarked(note_app->fSettingsMessage->FindBool("live_in_deskbar"));

	// Edit menu
	fEditMenu 		-> AddItem (fUndoItem 		= new BMenuItem(B_TRANSLATE("Can't Undo"), 	new BMessage(B_UNDO), 		'Z'));
	fUndoItem 		-> SetEnabled(false);		// I can't do undo without the message TEXT_CHANGED
	fEditMenu 		-> AddSeparatorItem();
	fEditMenu 		-> AddItem (fCutItem 		= new BMenuItem(B_TRANSLATE("Cut"), 			new BMessage(B_CUT), 		'X'));
	fCutItem 		-> SetTarget(this);
	fEditMenu 		-> AddItem (fCopyItem 		= new BMenuItem(B_TRANSLATE("Copy"), 		new BMessage(B_COPY), 		'C'));
	fCopyItem 		-> SetTarget(this);
	fEditMenu 		-> AddItem (fPasteItem 		= new BMenuItem(B_TRANSLATE("Paste"), 		new BMessage(B_PASTE), 		'V'));
	fPasteItem 		-> SetTarget(this);
	fEditMenu 		-> AddItem (fSelectAllItem 	= new BMenuItem(B_TRANSLATE("Select all"), 	new BMessage(B_SELECT_ALL), 'A'));
	fSelectAllItem 	-> SetTarget(this);

	// Font menu
	// Font: Size
	sizeFont 	= 	new BMenu (B_TRANSLATE("Size"));
	sizeFont 	-> 	SetRadioMode (true);
	fFontMenu 	-> 	AddItem (sizeFont);
	// Writing the menu...
	for (uint32 i = 0; i < sizeof(fontSizes) / sizeof(fontSizes[0]); i++ ){
		message = new BMessage (FONT_SIZE);
		message->AddFloat ("size", fontSizes[i]);
		char label[64];
		snprintf(label, sizeof(label), "%ld", fontSizes[i]);
		sizeFont->AddItem (menuItem = new BMenuItem (label, message));
		if (i == 3)
			menuItem->SetMarked(true);
	}

	// Font: Color
	colorFont =  new BMenu (B_TRANSLATE("Color"));
	colorFont->SetRadioMode (true);
	fFontMenu->AddItem (colorFont);
	// Writing the menu...
	for (uint32 i = 0; i < sizeof(colors) / sizeof(colors[0]); i++ ){

		message = new BMessage (FONT_COLOR);
		message->AddInt8 ("red", (int8)colors[i].red);
		message->AddInt8 ("green", (int8)colors[i].green);
		message->AddInt8 ("blue", (int8)colors[i].blue);

		label = "";
		switch (i) {
			case 0: {
				label = B_TRANSLATE("Black");
				color = black;
			}
			break;
			case 1: {
				label = B_TRANSLATE("Red");
				color = red;
			}
			break;
			case 2: {
				label = B_TRANSLATE("Green");
				color = green;
			}
			break;
			case 3: {
				label = B_TRANSLATE("Blue");
				color = blue;
			}
			break;
			case 4: {
				label = B_TRANSLATE("Yellow");
				color = yellow;
			}
			break;
			default:
				break;
		}
		colorFont->AddItem (menuItem = new ColorMenuItem (label, color, message));
			// Set the item as "in use"
			if (i == 0)
				menuItem->SetMarked(true);
	}

	fFontMenu->AddSeparatorItem();

	// Font type
	fCurrentFont = 0;

	be_plain_font->GetFamilyAndStyle (&plainFamily,&plainStyle);

	// Taking the number of font families
	numFamilies = count_font_families();
	for (int32 i = 0; i < numFamilies; i++) {

		// Getting the font families
		if (get_font_family(i, &family) == B_OK) {

			// Creating a menu of that font family
			fontMenu = new BMenu (family);
			fontMenu->SetRadioMode (true);	// I can set only one item as "in use"
			fFontMenu->AddItem (menuItem = new BMenuItem (fontMenu, new BMessage (FONT_FAMILY)));

			if (!strcmp (plainFamily,family)) {
				menuItem->SetMarked (true);
				fCurrentFont = menuItem;
			}
			// Number of styles of that font family
			numStyles = count_font_styles (family);

			// Creating a submenu about each of that styles
			for (int32 j = 0; j < numStyles; j++) {

				if (get_font_style(family,j,&style,&flags)==B_OK) {
					fontMenu->AddItem (menuItem = new BMenuItem (style, new BMessage(FONT_STYLE)));

					if (!strcmp (plainStyle, style))
						menuItem->SetMarked(true);

				}
			}
		}
	}

	// Add the menu to the window
	AddChild(fNoteMenuBar);

	// Create the Alarm, Choice, Background and Tags Window
	CreateOtherWindows();
}

// We are trying to avoid that two instances of the same window are opened together
void NoteWindow :: CreateOtherWindows(){

	// Initialization of the other window to NULL
	fTagsWindow = NULL;
	fColorWindow = NULL;
	fAlarmWindow = NULL;
	fChoiceWindow = NULL;

}


void
NoteWindow::_CreateNoteView(void)
{
	// The main view is created
	BRect frameView = Bounds();
	frameView.top = fNoteMenuBar->Bounds().Height() + 1;

	fNoteView = new NoteView(frameView, B_FOLLOW_ALL, false, this);

	// Text and Scroll View
	frameView = fNoteView->Bounds();
	frameView.right -= B_V_SCROLL_BAR_WIDTH + 1;
	frameView.bottom -= B_H_SCROLL_BAR_HEIGHT + 1;
	frameView.left = 0;

	BRect frameText = frameView;

	frameText.OffsetTo(B_ORIGIN);
	frameText.InsetBy(TEXT_INSET, TEXT_INSET);

	fNoteText = new NoteText(frameView, frameText, B_TRANSLATE("NoteText"), this);
	fNoteText->SetDoesUndo(true);
	fNoteText->MakeFocus();
	fNoteText->SetStylable(true);
	// ScrollView
	fScrollView = new BScrollView("scrollview", fNoteText, B_FOLLOW_ALL, 0,
		true, true, B_NO_BORDER);

	fNoteView->AddChild(fScrollView);

	//load color read from the settings file which was loaded in fSettingsMessage
	fNoteView->SetBackgroundColor(note_app->fSettingsMessage->GetColor("def_color", {255,240,113}));

	if(!fSaveMessage)
		MoveTo(200+(note_app->fWindowCount*20),200+( note_app->fWindowCount*20));
}


// Function for the changes in the "type of font"
void NoteWindow :: SetFontStyle (const char* fontFamily, const char* fontStyle) {
	// Variables
	BMenuItem 	*superItem;
	BMenuItem	*menuItem;
	BFont 		font;
	font_family oldFamily;
	font_style 	oldStyle;
	uint32 		sameProperties;
	rgb_color 	sameColor;
	BMenuItem 	*oldItem;

	fNoteText->GetFontAndColor (&font, &sameProperties, &sameColor);
	// Copying the current font family and font style
	font.GetFamilyAndStyle (&oldFamily, &oldStyle);

	if (strcmp (oldFamily, fontFamily)) {
		oldItem = fFontMenu->FindItem (oldFamily);

		if (oldItem != NULL)
			// Removing the check
			oldItem->SetMarked (false);
	}

	font.SetFamilyAndStyle (fontFamily, fontStyle);
	fNoteText->SetFontAndColor (&font);

	superItem = fFontMenu->FindItem (fontFamily);

		if (superItem != NULL )
			superItem->SetMarked (true);	// Check the one that was selected

	menuItem = fFontMenu->FindItem("Black");
	menuItem->SetMarked(true);
}

// Function that saves the note
status_t NoteWindow :: Save(BMessage *message) {
	// Variables
	BFile config;
	BFile settings;
	BDirectory	dir;
	BEntry		entry;
	entry_ref 	ref;
	const char 	*name;
	status_t 	err;

	if (!message){
		message = fSaveMessage;
	}

	// Saving the directory and the path of the note
	if ((err = message->FindRef("directory",&ref)) != B_OK)
		return err;
	if ((err = message->FindString("name", &name)) != B_OK)
		return err;

	// Set fRef
	err = dir.SetTo(&ref);
	if (err != B_OK)
		return err;

	err = entry.SetTo(&dir, name);
	if (err != B_OK)
		return err;

	if (fRef == NULL)
		fRef = new entry_ref;

	err = entry.GetRef(fRef);
	if (err != B_OK)
		return err;

	// Setting the title
	SetTitle(name);

	BMessage archive(*message);
	fNoteView->Archive(&archive, true);
	fNoteView->SetReplicated(false);

	if ((err = config.SetTo("/boot/home/config/settings/TakeNotes/config", B_READ_WRITE | B_CREATE_FILE)) != B_OK){
			return err;
		}
	else {
		save_settings(fSaveMessage, "config", "TakeNotes");
	}
	if (fSaveMessage != message) {
		delete fSaveMessage;
		fSaveMessage = new BMessage(*message);
	}

	return err;
}

// It reads the associetions between notes and applications
void NoteWindow :: _LoadDB(){
	// Variables
	BFile	config;
	off_t	length;
	char*	input;

	BString stringa;
	BString	path;
	BString signature;

	int32	firstComma;
	int32	lastComma;

	// Check if the file exists and if it is readable
	if (fDatabase.SetTo("/boot/home/config/settings/TakeNotes/config", B_READ_ONLY) != B_OK){
		printf("Errore!\n");
	}

	// Obtain the length of the file and sufficent memory is allocated for the file's contents
	fDatabase.GetSize(&length);
	input = (char *)malloc(length);

	// Read the database file
	if (input && (fDatabase.Read(input, length) >= B_OK)){
		stringa.SetTo(input);
		free(input);
	}	else {

		printf("errore di lettura file\n");
		return;
	}

	// Clean the string from random error
	stringa.Remove(stringa.FindLast(":")+1,stringa.CountChars());

	while (stringa.CountChars() > 0 ){

		// Searching for the first :
		firstComma = stringa.FindFirst(":")+1;

		// Removing the :
		stringa.MoveInto(path, 0, firstComma);
		path.RemoveLast(":");

		// Searching for the second :
		lastComma = stringa.FindFirst(":")+1;

		// Removing the :
		stringa.MoveInto(signature, 0, lastComma);
		signature.RemoveLast(":");

		fHash->AddNote(signature,path);
	}
}

// Functions that associates a note to an application
status_t NoteWindow :: _SaveDB(const char* signature){
	// Variable
	BFile		config;
	BEntry		entry;
	BPath		path;
	status_t	err;
	entry_ref	ref;
	const char*	name;
	off_t		length;
	BString		toWrite;
	int			countSignatures,
				countNotes,
				found = 0;
	char		*sig;
	BAlert 		*alert;

	if (signature){

		// Initialize a BEntry object starting from the informations stored in fSaveMessage
		if (( err = fSaveMessage->FindRef("directory",&ref)) != B_OK)
				return err;
		if (( err = fSaveMessage->FindString("name", &name)) != B_OK)
				return err;

		entry.SetTo(new BDirectory(&ref),name);

		// Finally obtain the BPath object that will allow us to retrain the path string
		path.SetTo(&entry);

		fHash = new AppHashTable();
		_LoadDB();

		// We take the number of the signatures...
		countSignatures = fHash->GetNumSignatures();

		// Searching if the signature is still present with this note
		for (int i = 0; i < countSignatures; i++) {
			sig = fHash->GetSignature(i);

			// Signature found?
			if (strcmp (signature, sig) == 0) {

				// Number of notes in this signature
				BString str(signature);
				countNotes = fHash->GetNumNotes(str);

				for (int j = 0; j < countNotes; j++)
					// Note found
					if (strcmp(fHash->GetNote(str, j), path.Path()) == 0) {
						found = 1;
						alert = new BAlert("Error", B_TRANSLATE("Error: This selection is in the database!"),
							B_TRANSLATE("OK"), NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
						alert->SetShortcut(0,B_ESCAPE);
						alert->Go();
					}
			}
		}

		if (found == 0) {
			// Open the config file, if it doesn't exist we create it
			if ((err = config.SetTo("/boot/home/config/settings/TakeNotes/config", B_READ_WRITE | B_CREATE_FILE)) != B_OK){
				return err;
			}

			// Prepare the string
			config.GetSize(&length);
			toWrite.Append(path.Path());
			toWrite.Append(":");
			toWrite.Append(signature);
			toWrite.Append(":");

			// Obtain the length of the file
			config.GetSize(&length);

			if (length == 0)
				config.Write (toWrite.String(), toWrite.Length());
			else
				// Write the new value
				config.WriteAt(length, toWrite.String(), toWrite.Length());

				// Unload the file and return
				config.Unset();
				return B_OK;
			}
		else
			return B_ERROR;

	}
	else
		return B_ERROR;

}

// Function for the reception of the messages
void NoteWindow :: MessageReceived(BMessage* message) {
	//	Variables
		BFont	  font;
  const char 	  *fontFamily,
  				  *fontStyle,
  				  *signature,
  				  *alertstr;
		char	  *param[1];
  		char	  stringa[2];
		char	  buffer[50];
  		void	  *ptr;
		int		  second,
				  minute,
				  hour,
				  day,
				  month,
				  year,
				  found,
				  mail,
				  countApps,
				  h;
		uint32	  sameProperties;
		int8	  c;
		int16 	  i;
		int32 	  k,
		          j,
		          length,
		          offset,
		          count = 0;
		float     fontSize;
		rgb_color colore,
			 	  sameColor;
		entry_ref reference;
		BAlert 	  *myAlert,
				  *alert;
		BList     *aList;
		app_info  *appInfo;
        team_id   who;
        BMenuItem *item;
        BMenu     *menu;
        struct tm *timeinfo;

	// Receiving the messages
	switch (message->what) {

		case LIVE_IN_DESKBAR: {
			if(fLiveInDeskbar->IsMarked()) {
				fLiveInDeskbar->SetMarked(false);
				BDeskbar *db = new BDeskbar();
				if (db->HasItem(kDeskbarItemName))
					db->RemoveItem(kDeskbarItemName);
			}	else {
				fLiveInDeskbar->SetMarked(true);
				note_app->_InstallReplicantInDeskbar();
			}

		note_app->fSettingsMessage->SetBool("load_last_note", fLoadLastNote->IsMarked());
		note_app->fSettingsMessage->SetBool("live_in_deskbar", fLiveInDeskbar->IsMarked());

		if (fRef)  {
			note_app->fSettingsMessage->RemoveData("last_note" );
			note_app->fSettingsMessage->AddRef("last_note", fRef );
		}
		if (save_settings(note_app->fSettingsMessage, "settings", "TakeNotes") == B_OK)
				printf("settings saved\n");
		}
		break;

		case LOAD_LAST_NOTE: {
			if(fLoadLastNote->IsMarked())
				fLoadLastNote->SetMarked(false);
			else
				fLoadLastNote->SetMarked(true);
		}

		note_app->fSettingsMessage->SetBool("load_last_note", fLoadLastNote->IsMarked());
		if(fRef)  {
			note_app->fSettingsMessage->RemoveData("last_note" );
			note_app->fSettingsMessage->AddRef("last_note", fRef );
		}
		if(save_settings(note_app->fSettingsMessage, "settings", "TakeNotes") == B_OK)
				printf("settings saved\n");

		break;

		// Show the panel that allows to save the note
		case SAVE_AS: {
			fSavePanel->Show();
		}
		break;

		// Save the changes
		case SAVE:{
			if (!fSaveMessage){
				 fSavePanel->Show();
			}
			else {
				Save(fSaveMessage);
			}
		}
		break;

		// Save the note
		case B_SAVE_REQUESTED: {
			if(Save(message) != B_OK) {
				printf("error saving message\n");
			}
		}
		break;

		// Close the application
		case QUIT_APPL: {
			Quit();
		}
		break;

		// Edit messages
		// Cut the selected text
		case B_CUT:
			fNoteText->Cut(be_clipboard);
		break;

		// Copy the selected text
		case B_COPY:
			fNoteText->Copy(be_clipboard);
		break;

		// Paste the selected text
		case B_PASTE:
			fNoteText->Paste(be_clipboard);
		break;

		// Select all the text
		case B_SELECT_ALL:
			fNoteText->SelectAll();
		break;

		// Message for the undo function
		case B_UNDO: {		// If I have received a B_UNDO message...
			if (fCanUndo)	//...and I can do "Undo"
				fUndoFlag = true;
			if (fCanRedo)	//...and I can do "Redo"
				fRedoFlag = true;

			fNoteText->Undo(be_clipboard);

		}
		break;
		// Message that tells if the text is changes (it is used for the "can't undo")
		case TEXT_CHANGED:	{

			if (fUndoFlag) {

				fCanUndo = false;
				fCanRedo = true;
				fUndoItem->SetLabel(B_TRANSLATE("Redo"));
				fUndoItem->SetEnabled(true);
				fUndoFlag = false;

			}
			else {
				fCanUndo = true;
				fCanRedo = false;
				fUndoItem->SetLabel(B_TRANSLATE("Undo"));
				fUndoItem->SetEnabled(true);
				fRedoFlag = false;
		    }
		    //eventualle check here if there's a string available to convert to a link
		}
		break;

		// Font size
		case FONT_SIZE: {

			if (message->FindFloat ("size", &fontSize) == B_OK){
				fNoteText->GetFontAndColor(&font, &sameProperties, &sameColor);
				font.SetSize(fontSize);
				fNoteText->SetFontAndColor (&font, B_FONT_SIZE);
			}

		}
		break;

		// Font Color
		case FONT_COLOR: {

			message->FindInt8("red", &c);
			colore.red = (uint8)c;
			message->FindInt8("green", &c);
			colore.green = (uint8)c;
			message->FindInt8("blue", &c);
			colore.blue = (uint8)c;

			// Setting a different color for the font
			fNoteText->GetFontAndColor(&font, &sameProperties);
			fNoteText->SetFontAndColor(&font,0,&colore);

		}
		break;

		// Font type
		case FONT_FAMILY: {

			fontFamily = NULL;
			fontStyle = NULL;

			// Setting the font family
			message->FindPointer ("source", &ptr);
			fCurrentFont = static_cast <BMenuItem*>(ptr);
			fontFamily = fCurrentFont->Label();
			SetFontStyle (fontFamily, fontStyle);

		}
		break;

		// Font style
		case FONT_STYLE: {

			fontFamily = NULL;
			fontStyle = NULL;

			// Setting the font style
			message->FindPointer ("source", &ptr);
			item = static_cast <BMenuItem*>(ptr);
			fontStyle = item->Label();
			menu = item->Menu();

			if (menu != NULL) {
				fCurrentFont = menu->Superitem();
				if (fCurrentFont != NULL)
					fontFamily = fCurrentFont->Label();
			}
			SetFontStyle (fontFamily, fontStyle);

		}
		break;

		// Mail and Browser: it opens BeZilla or Mail
		case GO_TO_LINK: {

			// Copying the selected link
			fNoteText->GetSelection(&k,&j);
			length = j - k + 1;

			// If nothing was selected we need to break
			if (length <= 1)
				break;

			offset = k;
			fNoteText->GetText(offset, length, buffer);

			//the OS should decide how links are opened, user can adjust settings via the file_type preferences
			BString cmd = buffer;

			// Verifying if the link passed is an email ...
			if(cmd.FindFirst("@") > 0) {
					cmd.Prepend("open mailto:");
					system(cmd.String());
					break;
			}
			// ... or a link
			if(cmd.FindFirst("www") > 0 || cmd.FindFirst("http") > 0) {
					cmd.Prepend("open ");
					system(cmd.String());
					break;
			}
		}
		break;

		// Adding the date
		case ADD_DATA: {

			// Getting the date
			day = gettime().tm_mday;
			month = gettime().tm_mon + 1;
			year = gettime().tm_year+1900;
			second = gettime().tm_sec;
			minute = gettime().tm_min;
			hour = gettime().tm_hour;

			//todo: thaflo 2021: read separator from OS wide preferences
			sprintf(stringa, "%02d/%02d/%02d - %02d:%02d:%02d", day,
					month, year, hour, minute, second);

			// Inserting the string
			fNoteText->MakeFocus();
			fNoteText->Insert(stringa);

		}
		break;

		// Show the window that allows the user to change the background color
		case SET_COLOR:{

			if (fColorWindow == NULL){
				fColorWindow = new ColorWindow(BRect(300,300,700,450),this, this->FindView("NoteText")->ViewColor());
				fColorWindow->Show();

			} else {
//				fColorWindow->Activate();
			}
		}
		break;

		// Background color changed
		case COLOR_CHANGED: {
			message->FindInt8("red", &c);
			colore.red = (uint8)c;
			message->FindInt8("green", &c);
			colore.green = (uint8)c;
			message->FindInt8("blue", &c);
			colore.blue = (uint8)c;
			fNoteView->SetBackgroundColor(colore);

		}
		break;

		// Allows you to add an association between a note and an application
		case SET_APP: {

			if (!fSaveMessage){
				alert = new BAlert("Warning",B_TRANSLATE("You must save your note before!"),B_TRANSLATE("Cancel"),B_TRANSLATE("Save"));

				if (alert->Go() == 1) {
					fSavePanel->Show();
				}

			} else {

				if (fChoiceWindow == NULL){

					// We have to adapt the height of the window in function of the number
					// of applications that are running
	        		aList = new BList;

				    // Obtaining the applications that are running
		    	    be_roster->GetAppList(aList);
			    	countApps = aList->CountItems();

		       		 h = countApps * 17 + 300;
		       		 printf("Height: %d\n", h);

					fChoiceWindow = new ChoiceWindow(BRect(300,300,800,h), this);
					fChoiceWindow->Show();

					delete aList;
					aList = NULL;

				} else
					fChoiceWindow->Activate();
			}

		}
		break;

		// Associate an application to the post-it
		case RADIO_CHECKED: {

			const char* stringa;

			message->FindString("signature", &stringa);
			// Save the new association into the config file
			_SaveDB(stringa);

		}
		break;

		// It allows you to set the tags of a note
		case SET_TAGS: {

			if (!fSaveMessage){
				alert = new BAlert("Warning",B_TRANSLATE("You must save your note before!"),B_TRANSLATE("Cancel"),B_TRANSLATE("Save"));
				if (alert->Go() == 1) {
					fSavePanel->Show();
				}
			} else {
				// It creates a window
				if (fTagsWindow == NULL){

					fTagsWindow = new TagsWindow(fSaveMessage, this);
					fTagsWindow->Show();

				} else
					fTagsWindow->Activate();
			}
		}
		break;

		// Setting the alarm with the window opened
		case SET_ALARM: {

			if (fAlarmWindow == NULL){

				fAlarmWindow = new AlarmWindow(BRect(300,300,730,450),this);
				fAlarmWindow->Show();

			}
			else
				fAlarmWindow->Activate();

		}
		break;

		// Setting the alarm in the data structure
		case ALARM_MSG: {

			alarm_set = true;

			message->FindInt16("year", &i);
			fData.Year = i;
			message->FindInt16("month", &i);
			fData.Month = i;
			message->FindInt16("day", &i);
			fData.Day = i;
			message->FindInt16("hour", &i);
			fData.Hour = i;
			message->FindInt16("minute", &i);
			fData.Minute = i;

		}
		break;

		// Check for alarm to show (after the user has set one)
		case CHECK_ALARM: {

			// First check if an alarm has been set
			if (!(alarm_set)) {
				break;
			}
			// Second check if the runner has been correctly started
			if (runner->InitCheck() < B_NO_ERROR) {
				break;
			}

			// Check if date/time saved in data struct comes after system time
			// Declaring two timers: rawtime represents system time, userTime represents user input saved in the struct
			time_t rawtime;
			time_t userTime;

			// Get the current time (number of seconds from the "epoch"), stores it in the timer
			time( &rawtime );

			// Convert time_t time value to a tm struct
			timeinfo = localtime ( &rawtime );

			// Fill the timeinfo struct with data saved in data struct
			timeinfo->tm_year = fData.Year - 1900;
			timeinfo->tm_mon = fData.Month - 1;
			timeinfo->tm_mday = fData.Day;
			timeinfo->tm_hour = fData.Hour;
			timeinfo->tm_min = fData.Minute;
			timeinfo->tm_sec = 0;

			// Convert from struct tm to data type time_t
			userTime = mktime(timeinfo);

			// Compare user time and system time
			if (difftime(userTime, time( &rawtime) ) < 0 ) {

				//thaflo: send a notification/ was an alert before
				BNotification notification(B_INFORMATION_NOTIFICATION);
				notification.SetGroup("TakeNotes");
				notification.SetTitle(Title());
				notification.SetContent(fNoteText->Text());
				notification.SetMessageID("takenotes_info");
				notification.Send();
				alarm_set = false;

			}
		}
		break;

		// It removes the color window
		case COLOR_CLOSE: {
			if (fSaveMessage) {
				this->PostMessage(SAVE);
			}
			fColorWindow = NULL;
		}
		break;

		// It removes the alarm window
		case ALARM_CLOSE: {
			fAlarmWindow = NULL;
		}
		break;

		// It removes the tag window
		case TAGS_CLOSE: {
			fTagsWindow = NULL;
		}
		break;

		// It removes the choice window
		case CHOICE_CLOSE: {
			fChoiceWindow = NULL;
		}
		break;

		// About menu
		case B_ABOUT_REQUESTED: {
			note_app->AboutRequested();
		}
		break;

		case SET_DEFAULT_COLOR: {
		//save the new default color
			rgb_color c = {10,10,10,10};
			note_app->fSettingsMessage->SetColor("def_color", message->GetColor("new_def_color", c));
			if(save_settings(note_app->fSettingsMessage, "settings", "TakeNotes") != B_OK)
				printf("settings not saved\n");
		}
		break;

		default:
			BWindow::MessageReceived(message);
	}
}

// Closing the window
bool NoteWindow :: QuitRequested(){

	Quit();
	return(true);
}

// Function that quits the window
void NoteWindow :: Quit(){

	note_app->CloseNote();
	BWindow::Quit();

}

