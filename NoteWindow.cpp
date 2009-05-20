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
 * Last revision: Eleonora Ciceri, 16th May 2009
 *
 * Description: TODO
 */


#include "NoteWindow.h"
#include "ColorMenuItem.h"

#include <Clipboard.h>
#include <Autolock.h>
#include <Application.h>
#include <Roster.h>
#include <MessageRunner.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

// Constants
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
#define GO_TO_LINK 'gtlk'
#define ABOUT 'bout'
#define CHECK_ALARM 'chal'

#define MENU_BAR_HEIGHT 18;
#define TEXT_INSET 10

// Structures
const struct tm gettime() {
    time_t    t=time(NULL);
    return *localtime(&t);
}

// Flags

bool alarm_set = false;

// Constructor
NoteWindow::NoteWindow(BRect frame)
	: BWindow (frame, "TakeNotes", B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS){
	
	// Variables
	BMessage	*message;
	
	BMessage	*check_alarm_msg;
	
	bigtime_t 	interval = 10000;
		
	BRect 		menuBarRect,
				frameView,
				frameText;
				
	BMenuItem 	*menuItem = NULL;
	
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
				red = {255,0,0},
				green = {0,255,0},
				blue = {0,0,255},
				yellow = {254,254,92},
				color;
	rgb_color   colors[] = {black, red, green, blue, yellow};
	
		
	// Data of the data structure
	fDati.Id = 1;
	fDati.Titolo = "Titolo";


	// Initialize the messenger: the handler is the window itself
	
	fMessenger = BMessenger(this);
	
	// Create an empty message
	
	check_alarm_msg = new BMessage(CHECK_ALARM);
	
	// Initialize the message runner
	
	runner = new BMessageRunner(fMessenger, check_alarm_msg, interval);
		
	
	// Undo flags
	fCanUndo = false;		// If there's no text I can't do undo
	fUndoFlag = false;
	
	
	// Menu bar	
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
	
	/*************** Menu Item ***************/
	
	// Settings	
	fSettingsMenu -> AddItem (fChangeBackgroundColorItem = new BMenuItem ("Change background color",
			new BMessage (MENU_CHANGE_COLOR)));
	fSettingsMenu -> AddItem (fChangeBackgroundColorItem = new BMenuItem ("Add date and time",
			new BMessage (ADD_DATA)));
	fSettingsMenu -> AddItem (fSetAlarmItem = new BMenuItem ("Set alarm",
			new BMessage (SET_ALARM)));
	fSettingsMenu -> AddItem (fLink = new BMenuItem ("Go to the selected link...", new BMessage (GO_TO_LINK)));					
	
	// Edit

	fEditMenu -> AddItem (fUndoItem = new BMenuItem("Can't Undo", new BMessage(B_UNDO), 'Z'));
	fUndoItem -> SetEnabled(false);		// I can't do undo without the message TEXT_CHANGED
	
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
	
	for (uint32 i = 0; i <= sizeof(fontSizes) / sizeof(fontSizes[0]); i++ ){
		message = new BMessage (FONT_SIZE);
		message -> AddFloat ("size", fontSizes[i]);
		
		if (i != sizeof(fontSizes) / sizeof(fontSizes[0]) ){
			char label[64];
			snprintf(label, sizeof(label), "%ld", fontSizes[i]);
			sizeFont -> AddItem (menuItem = new BMenuItem (label, message));
		}
		
			if (i == 3)
				menuItem -> SetMarked(true);
	}
	
	// Font: Color
	colorFont = new BMenu ("Color");
	colorFont -> SetRadioMode (true);
	fFontMenu -> AddItem (colorFont);
	// Printing the menu...	
	delete message;
	
	for (uint32 i = 0; i < sizeof(colors) / sizeof(colors[0]); i++ ){

		message = new BMessage (FONT_COLOR);
		message -> AddInt8 ("red", (int8)colors[i].red);
		message -> AddInt8 ("green", (int8)colors[i].green);
		message -> AddInt8 ("blue", (int8)colors[i].blue);
		
		label = NULL;
		switch (i) {
			case 0: {
				label = "Black";
				color = black;
			}
			break;
			case 1: {
				label = "Red";
				color = red;
			}
			break;
			case 2: {
				label = "Green";
				color = green;
			}
			break;
			case 3: {
				label = "Blue";
				color = blue;
			}
			break;
			case 4: {
				label = "Yellow";
				color = yellow;
			}
			break;
			default:
				break;
		}
		colorFont -> AddItem (menuItem = new ColorMenuItem (label, color, message));
			// Set the item as "in use"
			if (i == 0)
				menuItem -> SetMarked(true);
	}
	
	fFontMenu -> AddSeparatorItem();
	
	// Font type	
	fCurrentFont = 0;
	
	be_plain_font -> GetFamilyAndStyle (&plainFamily,&plainStyle);
	
	numFamilies = count_font_families();
	for (int32 i = 0; i < numFamilies; i++) {
		
			if (get_font_family(i,&family) == B_OK) {
				fontMenu = new BMenu (family);
				fontMenu -> SetRadioMode (true);	// I can set only one item as "in use"
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
	
	// About menu
	fAboutMenu -> AddItem (menuItem = new BMenuItem ("About TakeNotes...", new BMessage (ABOUT)));	
	
	// Main view	
	frameView = Bounds();
	
	frameView.top = fNoteMenuBar->Bounds().Height() + 1;
	
	
	fNoteView = new NoteView (frameView, "TakeNotes"); // Useful for the "About"


	frameView = fNoteView -> Bounds();	
	frameView.top += 10;
	frameView.right -= B_V_SCROLL_BAR_WIDTH;
	frameView.bottom -= B_H_SCROLL_BAR_HEIGHT;
	frameView.left = 0;
	frameText = frameView;
	
	frameText.OffsetTo(B_ORIGIN);
	frameText.InsetBy(TEXT_INSET, TEXT_INSET);
	
	fNoteText = new NoteText(frameView, frameText, "NoteText", this);
	fNoteText->SetDoesUndo(true);
	fNoteText->MakeFocus(); 
	fNoteText->SetStylable(true);

	// ScrollView
	fScrollView = new BScrollView("scrollview", fNoteText, B_FOLLOW_ALL, 0, true, true, B_NO_BORDER);
	
	
	// It will be associated to the window
	AddChild(fNoteMenuBar);
	AddChild(fNoteView);
	fNoteView -> AddChild(fScrollView);

	Show();
	
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
	
	
	fNoteText -> GetFontAndColor (&font, &sameProperties, &sameColor);
	font.GetFamilyAndStyle (&oldFamily, &oldStyle); // Copying the current font family and font style
	if (strcmp (oldFamily, fontFamily)) {		
		BMenuItem *oldItem = fFontMenu -> FindItem (oldFamily);
		if (oldItem != NULL)
			oldItem -> SetMarked (false);	// Removing the check
	}		
	font.SetFamilyAndStyle (fontFamily, fontStyle);
	fNoteText -> SetFontAndColor (&font);
	
	
	superItem = fFontMenu -> FindItem (fontFamily);
		if (superItem != NULL )
			superItem -> SetMarked (true);	// Ckeck the one that was selected
	menuItem = fFontMenu -> FindItem("Black");
	menuItem -> SetMarked(true);
}

// Function for the reception of the messages
void NoteWindow :: MessageReceived(BMessage* message) {
	//	Variables
		BFont	  font;
		BRect	  aRect;	
	
  const char 	  *fontFamily,
  				  *fontStyle,
  				  *signature;
  		
		char	  *param[1];
  		char	  stringa[2];		  
  		void	  *ptr;		  
	
		int		  second,
				  minute,
				  hour,
				  day,
				  month,
				  year,
				  found,
				  mail;
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
		
		BAlert 	  *myAlert;
		
		BList     *aList;
		
		app_info  *appInfo;
        		
        team_id         who;
			  
	// Receiving the messages...	
	switch (message -> what) {
	
		case MENU_CHANGE_COLOR:{	
				
			aRect.Set(300,300,700,650);
			
			// Checking if there are more than one color window		
			if (be_app->CountWindows() == 1)
				fColorWindow = new ColorWindow(aRect,this);
			}
			break;
				
		// Background color
		case COLOR_CHANGED: {
				
			message->FindInt8("red", &c);
			colore.red = (uint8)c;
			message->FindInt8("green", &c);
			colore.green = (uint8)c;
			message->FindInt8("blue", &c);
			colore.blue = (uint8)c;
			fNoteView -> SetBackgroundColor(colore);
			
			// DATA STRUCTURE
			fDati.ColoreBack = colore;
			
		}
		break;	
		
		// About
		case ABOUT: {
			myAlert = new BAlert("About TakeNotes", 
				"Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved", 
				"OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
			myAlert -> Go();		
		}
		break;
		
		// Mail and Browser
		case GO_TO_LINK: {
		
		        // Copying the selected link
				fNoteText -> GetSelection(&k,&j);
				length = j - k + 1;
				offset = k;
				fNoteText -> GetText(offset, length, param[0]);
				
				found = 0;
				
				// Verifying if the link passed is an email or a link
				for (count = 0; param[0][count] != NULL; count++){
					if(param[0][count] == '@') {
						found = 1;
						break;
					}
				}
								
				if (found == 0) {
					// Signature of BeZilla
					signature = strdup("application/x-vnd.Mozilla-Firefox");
					mail = 0;
				}
				else {
					// Signature of mail
					signature = strdup("application/x-vnd.Be-MAIL");
					mail = 1;
				}
				
				// We write "mailto:mail" as required by the program "Mail"
				if (mail == 1) {
					char mail[100] = "";
					
					fNoteText -> GetSelection(&k,&j);
					length = j - k + 1;
					offset = k;
					fNoteText -> GetText(offset, length, mail);
					
					/***************************************************/
					// CRASH:PERCHE'???????????????????
					sprintf(param[0], "mailto:%s", mail);
					/***************************************************/
				}
				
				// Flag
				found = 0;
				
				// Initialization
        		aList = new BList;
        		appInfo = new app_info();

		        // Obtaining the applications that are running
		        be_roster->GetAppList(aList); 

					// We look for the current instances of BeZilla that are running
	       			for (count=0;count< aList->CountItems();count++){

		                who = (team_id)aList->ItemAt(count);
   		 	            be_roster->GetRunningAppInfo(who,appInfo);
   		 	            
   		 	            // Is there an instance that is running?
        		        if (strcmp (appInfo->signature, signature) == 0) {
							myAlert = new BAlert("BeZilla is opened", 
								"Close the browser first, then open this link", 
								"OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
							myAlert -> Go();
							found = 1;
							break;
						}
        			} 
        			
        			// BeZilla is not running 
        			if (found == 0) {
				
						// We launch the application
						be_roster -> Launch(signature, 1, param, NULL);
        			}
								
		}
		break;
		
		// Font size
		case FONT_SIZE: {
		
			if (message -> FindFloat ("size", &fontSize) == B_OK){		
				fNoteText -> GetFontAndColor(&font, &sameProperties, &sameColor);
				font.SetSize(fontSize);
				fNoteText -> SetFontAndColor (&font, B_FONT_SIZE);
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
			
			fNoteText -> GetFontAndColor(&font, &sameProperties);
			fNoteText -> SetFontAndColor(&font,0,&colore);
		}
		break;
		
		// Font type
		case FONT_FAMILY: {
		
			fontFamily = NULL;
			fontStyle = NULL;
			
			message -> FindPointer ("source", &ptr);
			fCurrentFont = static_cast <BMenuItem*>(ptr);
			fontFamily = fCurrentFont -> Label();
			SetFontStyle (fontFamily, fontStyle);
		}
		break;
		
		// Font style
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
		
		//Edit messages		
		case B_CUT:
			fNoteText -> Cut(be_clipboard);
		break;
		
		case B_COPY:
			fNoteText -> Copy(be_clipboard);
		break;
		
		case B_PASTE:
			fNoteText -> Paste(be_clipboard);
		break;
		
		case B_SELECT_ALL:
			fNoteText -> SelectAll();
		break;
		
		// Message that tells if the text is changes (it is used for the "can't undo")
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
		    // Modifying the data structure
		    fDati.Contenuto = (char*)fNoteText -> Text();
		break;
				
		// Message for the undo function	
		case B_UNDO:		// If I have received a B_UNDO message...
		if (fCanUndo)	//...and I can do "Undo"
			fUndoFlag = true;
		if (fCanRedo)	//...and I can do "Redo"
			fRedoFlag = true;
			
		fNoteText -> Undo(be_clipboard);
		break;
		
		// Adding the date
		case ADD_DATA: {
			day = gettime().tm_mday;
			month = gettime().tm_mon + 1;
			year = gettime().tm_year+1900;
			second = gettime().tm_sec;
			minute = gettime().tm_min;
			hour = gettime().tm_hour;
			
			
			sprintf(stringa, "%d/%d/%d - %d:%d:%d", day,
					month, year, hour, minute, second);
			
			fNoteText -> MakeFocus();
			fNoteText -> Insert(stringa);
		}
		break;
		
		// Setting the alarm with the window opened
		case SET_ALARM: {
			
			aRect.Set(300,300,800,600);
			fAlarmWindow = new AlarmWindow(aRect,this);
		}		
		break;
		
		// Setting the alarm in the data structure
		case ALARM_MSG: {
				
			alarm_set = true;
				
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
		
		// Check for alarm to show (after the user has set one)
		case CHECK_ALARM: {
		
			// First check if an alarm has been set
			
			if(!(alarm_set)) {
				break;
			} 
			
			// Second check if the runner has been correctly started
			
			if(runner -> InitCheck() < B_NO_ERROR) {		
				break;			
			}
			
		
			// Check if date/time saved in data struct comes after system time
			
			// Declaring two timers: rawtime represents system time, userTime represents user input saved in the struct
	
			time_t rawtime;
			time_t userTime;
	
			// Declare a time struct
	
			struct tm *timeinfo;
	
			// Get the current time (number of seconds from the "epoch"), stores it in the timer
		
			time( &rawtime );
	
			// Convert time_t time value to a tm struct
	
			timeinfo = localtime ( &rawtime );
		
			// Fill the timeinfo struct with data saved in data struct
	
			timeinfo -> tm_year = fDati.Anno - 1900;
			timeinfo -> tm_mon = fDati.Mese - 1;
			timeinfo -> tm_mday = fDati.Giorno;
			timeinfo -> tm_hour = fDati.Ora;
			timeinfo -> tm_min = fDati.Minuto;
			timeinfo -> tm_sec = 0;	
	
			// Convert from struct tm to data type time_t
	
			userTime = mktime(timeinfo);	

			// Compare user time and system time
	
			if( difftime(userTime, time( &rawtime) ) < 0 ) {
			
				myAlert = new BAlert("Alarm activated", "NOTICE: an alarm has been activated!", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
				myAlert -> Go();
				
			// WARNING!!!! We have to think about this: when an alarm has been activated, remember to "delete" it otherwise the alarm will appear over and over
				
				alarm_set = false;

			}				
		}
		break;
			
		default:
			BWindow::MessageReceived(message);
	}
}
	
// Closing the window
bool NoteWindow::QuitRequested(){

	be_app->PostMessage(B_QUIT_REQUESTED);
	return(true);
}
