/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 * 
 * Last revision: Ilio Catallo, 3rd June 2009
 *
 * Description: Header of the tags window
 */
 
 #ifndef TAG_WINDOW_H
 #define TAG_WINDOW_H
 
 // Libraries
 #include <Button.h>
 #include <File.h>
 #include <TextControl.h>
 #include <Window.h>
 
// Window that allows you to add tags
class TagsWindow : public BWindow{
 
	public:
 	
 						TagsWindow(BMessage *fSaveMessage, BHandler *handler);
 						~TagsWindow();
 		virtual void	MessageReceived(BMessage *message);	
 		virtual bool	QuitRequested();
 		virtual	void	Quit();
 
 	private:
 	
 		BButton			*fDoneButton;
 		BButton			*fUndoButton;
 		BTextControl 	*fTag1;
		BTextControl 	*fTag2;
		BTextControl    *fTag3;
		
		BFile			fFile;
		
		BMessenger		*fMessenger;	
};
 
#endif
