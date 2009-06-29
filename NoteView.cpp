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
 * Last revision: Ilio Catallo, 29th June 2009
 *
 * Description: View of the note. It can be implemented in three instances:
 *				- the view in the deskbar is the controller of the application
 *				- the view in the window, that is the container of the NoteText
 *				- the view as a replicant placed on a shelf (eg: the desktop)
 */

// Our libraries
#include "NoteApplication.h"
#include "NoteView.h"

// Other libraries
#include <Application.h>
#include <Alert.h>
#include <Deskbar.h>
#include <Dragger.h>
#include <File.h>
#include <FilePanel.h>
#include <IconUtils.h>
#include <MenuItem.h>
#include <NodeInfo.h>
#include <PopUpMenu.h>
#include <String.h>
#include <Resources.h>
#include <Roster.h>

// System libraries
#include <stdio.h>
#include <stdlib.h>

// Messages
#define FONT_BOLD 			'fntb'
#define CHOOSE_APPL			'cspp'
#define	OPEN_TAKENOTES		'onwp'
#define	OPEN_FILE			'onfl'
#define REMOVE_ASSOCIATION	'rmsc'

// Constructor
NoteView :: NoteView(BRect frame, int32 resizingMode, bool inDeskbar, BHandler *handler)
	   	 : BView(frame, "TakeNotes", resizingMode, B_FRAME_EVENTS | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	   		fInDeskbar(inDeskbar){
	   	   	   	   
	// Variable
	BDragger *dragger;	
		
	// Initilization
	fBitmap = NULL;
		
	if (handler) 
		fMessenger = new BMessenger(handler); 
	fHash = new AppHashTable();
		
	// Graphical Stuff				
	if (!inDeskbar){
		
		// We don't have to add the dragger if the view is in the deskbar
		SetViewColor(254,254,92,255); 
		dragger = new BDragger(BRect(0,0,7,7),this,B_FOLLOW_NONE);
		AddChild(dragger);
		dragger->SetViewColor(ViewColor());
		
	} 
	else {
			
		fReplicated = false;
		printf("sono nella deskbar\n");
		InitBitmap();
		
	}		   
}

// Constructor
NoteView :: NoteView (BMessage *msg, BHandler *handler)
		   : BView(msg){
			
	// Variables
	app_info	info;
			
	// Initialization
	fReplicated = true;
	if (handler) 
		fMessenger = new BMessenger(handler);
			
	// Check if we are in the deskbar
	if (be_app->GetAppInfo(&info) == B_OK && !strcasecmp(info.signature, "application/x-vnd.Be-TSKB")){
		fInDeskbar = true;
		fReplicated = false;

	} 
	else 
		printf("%s\n",info.signature);		
}

// Destructor
NoteView :: ~NoteView(){}
		
// Function used to draw in the view
void NoteView :: Draw (BRect update){

	printf("fReplicated: %d\n",fReplicated);	

	if (fReplicated || !fInDeskbar)
		return;
		
	SetDrawingMode(B_OP_ALPHA);
	DrawBitmap(fBitmap);
	SetDrawingMode(B_OP_COPY);

}

// Function InitBitmap
void NoteView :: InitBitmap(){

	// Variables
	image_info 	info;
	BFile		file;			
	const 		void*		data = NULL;
	size_t		size;
	BBitmap 	*icon;
			
	// Initialization
	if (fBitmap){
		delete fBitmap;
	}
			
	fBitmap = NULL;
			
	// Try to locate ourselves on the FS and open in read only
	if (our_image(info) != B_OK)
		printf("errore nell'our_image\n");
			
	file.SetTo(info.name,B_READ_ONLY);
	if (file.InitCheck() < B_OK)
		printf("errore nell'apertura file\n");
				
	// Loading the resourced linked in the binary
	BResources resources(&file);
	if (resources.InitCheck() < B_OK)
		printf("errore nel caricare le risorse\n");
			
	// Load the vectorial icon into a void pointer
	data = resources.LoadResource(B_VECTOR_ICON_TYPE,1,&size);
	if (data != NULL){
			
		//Obtain the icon from the blob
		icon = new BBitmap(Bounds(), B_RGBA32);
					
		if (icon->InitCheck() == B_OK && BIconUtils::GetVectorIcon((const uint8 *)data,size,icon) == B_OK){
					
			printf("sono riuscito a caricare l'icona!\n");
			fBitmap = icon;
			Invalidate();						
					
		} 
		else {
					
			printf("Non sono riuscito a caricare l'icona!\n");
			delete icon;
								
		}
			
		} 
		else
			printf("i dati erano vuoti\n");
}

void NoteView :: AttachedToWindow(){

	BView :: AttachedToWindow();
		
	/*
	* if there's a parent it means we're in the deskbar
	* so we set the background color the same as the deksbar
	* in order to obtain the alpha trasparency for the icon
	*/
	if (Parent() && !fReplicated){
		SetViewColor(Parent()->ViewColor());
		SetLowColor(ViewColor());

	}
}

// Function that reacts when someone clicks on the view
void NoteView :: MouseDown(BPoint point){

	// Variables
	BList 		*aList;
	app_info 	*appInfo;
    int 		count;
    entry_ref	ref;
    BString		name;
    BPopUpMenu  *menu;
    int 		countSignatures;
    char 		*sig,
    			*note;
    BMenu 		*subMenu,
    			*noteMenu;
    BMessage 	*mess,
    			*messRemove;
	

	if (fInDeskbar && !fReplicated) {
	
		// Load the database of notes
		fHash = new AppHashTable();
		_LoadDB();

		menu = new BPopUpMenu(B_EMPTY_STRING, false, false);
		menu->SetAsyncAutoDestruct(true);
		menu->SetFont(be_plain_font);

		menu->AddItem(new BMenuItem("Open TakeNotes" B_UTF8_ELLIPSIS,new BMessage(OPEN_TAKENOTES)));
		menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED)));
		if (fHash->HasElement()) menu->AddSeparatorItem();

				
		// Initialization
    	aList = new BList;
    	appInfo = new app_info();

	   	// Obtaining the applications that are running
   		be_roster->GetAppList(aList); 

		// We look for the current instances that are running
		for (count=0;count< aList->CountItems();count++){
   	    	team_id who = (team_id)aList->ItemAt(count);
   	  		be_roster->GetRunningAppInfo(who,appInfo);
  		 	            	
         	
       		// Show the name of the running program istead of its signature
        	ref = appInfo->ref;
   			name.SetTo(ref.name);
   		 	            	
   			// Not show the system process like the *_server program or the Deskbar / Tracker
   			if (name.Compare("registrar") != 0 && name.Compare("Tracker") != 0 && name.Compare("Deskbar") != 0 &&
   		 		name.Compare("net_server") != 0 && name.Compare("syslog_daemon") !=0 && name.Compare("input_server") != 0 &&
   		 		name.Compare("midi_server") !=0 && name.Compare("print_server") !=0 && name.Compare("media_server") != 0 &&
   		 		name.Compare("media_addon_server") != 0 && name.Compare("afp_server") != 0 && name.Compare("debug_server") != 0) {
   		 	
   		 		// Obtain the signature number
   				countSignatures = fHash -> GetNumSignatures();
   			
   				// For each signature in the list we check if it is the hash table and in case we add it to the menu
   				for (int i = 0; i < countSignatures; i++) {
   					sig = fHash -> GetSignature(i);
   					if (strcmp (appInfo -> signature, sig) == 0) {
   						subMenu = new BMenu (ref.name);
   					
   						// Make the list of the notes releated to that application
   						int countNotes = fHash -> GetNumNotes(sig);
   						for (int j = 0; j < countNotes; j++) {
   							note = fHash -> GetNote(sig, j);
   							
   							//Messing with pointers
   							team_id *ptrWho = &who;
   							
   							mess = new BMessage(OPEN_FILE);
   							mess -> AddString("Note", note);
   							mess -> AddPointer("team",(void *)ptrWho); // We pass also the team_id in order to focus the application
   						
   							messRemove = new BMessage (REMOVE_ASSOCIATION);
   							messRemove -> AddString ("Note", note);
   							messRemove -> AddString ("Signature", sig);
   							
   							noteMenu = new BMenu (note);
   							noteMenu -> AddItem (new BMenuItem ("Open", mess));
   							noteMenu -> AddItem (new BMenuItem ("Remove the association", messRemove));
   							noteMenu -> SetTargetForItems(this);
   							subMenu -> AddItem (noteMenu);
   						}
   					
   					
   						subMenu -> SetTargetForItems(this);
   						menu -> AddItem (subMenu);
   					}
   				}
   		 	
   		 
   			}   		 	
				
     	}  

		menu->SetTargetForItems(this);

		ConvertToScreen(&point);
		menu->Go(point, true, true, true);
	}

}
		
// Function Archive		
status_t NoteView :: Archive (BMessage *msg,bool deep) const{
				
	// Variables
	BFile 		file;
	BNodeInfo 	nodeinfo;
	BString		string;
	entry_ref	ref;
	status_t	err;
	const char	*name;
	BDirectory	dir;
		
	BView ::Archive(msg,deep);
		
	// Add the information needed to restore the view as a replicant
	msg->AddString("add_on","application/x-vnd.ccc-TakeNotes");
	msg->AddString("class","NoteView");
		
	// If the function has been called by NoteWindow::Save we should find this information and save 
	if (msg->FindRef("directory",&ref) == B_OK && msg -> FindString("name", &name) == B_OK) {
			
		dir.SetTo(&ref);
		if ((err = dir.InitCheck()) != B_OK)
			return err;
			
		// Create the note on the FS
		file.SetTo(&dir, name, B_READ_WRITE | B_CREATE_FILE);
		
		// File the note with the current view
		msg->Flatten(&file);
		
		// Add the MIME type information
		nodeinfo.SetTo(&file);
		nodeinfo.SetType("application/takenotes");		
		
		file.Unset();		
			
	}	
		
	return B_OK;		
}

// It istantiate a new view
BArchivable* NoteView :: Instantiate(BMessage *msg){

		return new NoteView(msg);
		
}

// Menu aboutrequested. it contains some information about us and our work.
void NoteView :: AboutRequested(){

	// Variable
	BAlert *alert;

	alert = new BAlert("About TakeNotes", 
			"Copyright 2009\n\nIlio Catallo,\nStefano Celentano,\nEleonora Ciceri.\n\nall rights reserved, distribuited under the terms of the GPLv2 license\n\nIcons by Meanwhile", 
			"OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
	alert->SetShortcut(0,B_ESCAPE);
	alert->Go();

}

// Functions that associates a note to an application
status_t NoteView :: _SaveDB(){
		
	// Variable
	BFile		config;
	BEntry		entry;
	BPath		path;
	status_t	err;
	entry_ref	ref;
	BString		toWrite;
	int			countSignatures,
				countNotes;
			
	printf("-----------------------------\n");
	fHash->PrintToStream();
	printf("-----------------------------\n");		
			
	int cs = fHash -> GetNumSignatures();
	for (int i = 0; i < cs; i++) {
		char* sig = fHash -> GetSignature(i);
		printf("Signature: %s\n", sig);
		int cn = fHash -> GetNumNotes(sig);
				
		for (int j = 0; j < cn; j++) {
			char* note = fHash -> GetNote(sig, j);
			printf(" - %s\n", note);
		}
		printf("\n");
	}
	
	
	if ((err = config.SetTo("/boot/home/config/settings/TakeNotes/config", B_READ_WRITE |  B_CREATE_FILE | B_ERASE_FILE)) != B_OK){
		return err;
	}
	

	toWrite.SetTo("");
	// Writing the structure
	countSignatures = fHash -> GetNumSignatures();
	for (int i = 0; i < countSignatures; i++) {
		
		// Prepare the string 
		char* signature = fHash -> GetSignature(i);
		countNotes = fHash -> GetNumNotes(signature);
		
		
		printf("(FILE) nodo: %d, signature: %s\n",i,signature);
		
		for (int j = 0; j < countNotes; j++) {
			char* note = fHash -> GetNote (signature, j);
			printf(">>(FILE) nota: %d, path: %s\n", j, note);
			toWrite.Append(note);
			toWrite.Append(":");
			toWrite.Append(signature);
			toWrite.Append(":");		
		}
		
		
		
		/*//Write the new value
		//config.WriteAt(0,toWrite.String(), toWrite.Length());
						
		//Obtain the length of the file
		config.GetSize(&length);
				
		if (length == 0)
			config.Write (toWrite.String(), toWrite.Length());	
		else				
			//Write the new value
			config.WriteAt(length, toWrite.String(), toWrite.Length());
		*/
		
	}
	printf("\n(SAVE_DB) %s\n",toWrite.String());
	config.Write (toWrite.String(), toWrite.Length());	
	//Unload the file and return
	config.Unset();
	return B_OK;

}

// Function that analyzes the messages received
void NoteView :: MessageReceived(BMessage *message){
				
	//Variables
	char	*argv[1];

	void **c;
	void *b;
	team_id	*a;	

	message->PrintToStream();

	switch(message->what){
		
		case B_ABOUT_REQUESTED:
			AboutRequested();
		break;
				
		case OPEN_TAKENOTES:
			_OpenTakeNotes();
		break;
				
		case OPEN_FILE:{

			//Find the note path and the team_id of the releated application	
			argv[0] = (char*)message -> FindString("Note");
			message -> FindPointer("team", c);
			
			b = &c;
			a = (team_id *)b;
					
			//Open the note and make focus on the releated application
			be_roster -> Launch("application/x-vnd.ccc-TakeNotes", 1, argv, NULL);
			be_roster -> ActivateApp(*a);
			
		}
		break;
		
		// Remove the association between a note and an application
		case REMOVE_ASSOCIATION: {

			// Removing from the data structure
			BString n(message->FindString("Note"));
			BString s(message->FindString("Signature"));
			
			printf("PRIMA DELLA PARTENZA: %s\n",n.String());
		
			fHash -> DeleteNote (s, n);
			
			// Rewriting the file
			_SaveDB();
		};
		break;
			
		case B_QUIT_REQUESTED:
			_Quit();
		break;
					
		default:
			BView::MessageReceived(message);
		break;
			
	}
}

// Function used to load the database of the associations between an application and a note
void NoteView :: _LoadDB(){

	// Variables
	BFile	config;
	off_t	length;
	char	*input;

	BString stringa;
	BString	path;
	BString signature;
	
	int32	firstComma;
	int32	lastComma;
			
	// Check if the file exists and if it is readable
	if (fDatabase.SetTo("/boot/home/config/settings/TakeNotes/config", B_READ_ONLY) != B_OK){

	}
		
	//Obtain the length of the file and sufficent memory is allocated for the file's contents
	fDatabase.GetSize(&length);
	input = (char *)malloc(length);
	
	// Actually read the database file
	if (input && (fDatabase.Read(input, length) >= B_OK)){
		
		stringa.SetTo(input);
		free(input);	
		
	} else {
		
		printf("errore di lettura file\n");
		return;
			
	}
		
	fDatabase.Unset();
	// Clean the string from random error
	//printf("(LOAD_DB) %s\n",stringa.String());
	stringa.Remove(stringa.FindLast(":")+1,stringa.CountChars());
	//printf(">>(LOAD_DB) %s\n\n",stringa.String());
		
	while (stringa.CountChars() > 0 ){
	
		// We look for the initial point
		firstComma = stringa.FindFirst(":")+1;
		
		// Catch the path and remove the :
		stringa.MoveInto(path, 0, firstComma);
		path.RemoveLast(":");
		
		// Search for the second :
		lastComma = stringa.FindFirst(":")+1;
		
		// Catch the signature and remove the :
		stringa.MoveInto(signature, 0, lastComma);
		signature.RemoveLast(":");
		
		printf(">>(LOAD_DB)path: %s\n",path.String());
		printf(">>(LOAD_DB)signature: %s\n",signature.String());
		//printf(">>(LOAD_DB)todo: %s\n\n",stringa.String());
		
		fHash->AddNote(signature,path);
	}

	printf("(LOADDB)------------------------\n");
	fHash->PrintToStream();
	printf("(LOADDB)-------------------------\n");

}

// Launch TakeNotes
void NoteView :: _OpenTakeNotes(){

	be_roster->Launch("application/x-vnd.ccc-TakeNotes");
	
}

// Quit the application
void NoteView :: _Quit(){

	if (fInDeskbar){
	
		BDeskbar deskbar;
		deskbar.RemoveItem(kDeskbarItemName);
	
	} else {
	
		be_app->PostMessage(B_QUIT_REQUESTED);
    }

}

// Set the flag fReplicated
void NoteView :: SetReplicated(bool flag){

	fReplicated = flag;

}

// Get the flag fRelpicated
bool NoteView :: GetReplicated(){

	return fReplicated;
		
}

// Set the background color of the view
void NoteView :: SetBackgroundColor(rgb_color color){
	
	SetViewColor(color);
	ChildAt(0)->SetViewColor(ViewColor());
	
	//Find the text view and change the color
	if (FindView("NoteText") != NULL){
	
		FindView("NoteText")->SetViewColor(color);
		
		// Tell the view to refresh itself
		FindView("NoteText")->Invalidate();
		ChildAt(0)->Invalidate();
		Invalidate();
	}
	
	

}

extern "C" _EXPORT BView *instantiate_deskbar_item(void){

	return new NoteView(BRect(0, 0, 15, 15), B_FOLLOW_LEFT | B_FOLLOW_TOP,true);
}
