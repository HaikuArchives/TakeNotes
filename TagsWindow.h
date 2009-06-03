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
 * Description: TODO
 */
 
 #ifndef TAG_WINDOW_H
 #define TAG_WINDOW_H
 
 #include <Button.h>
 #include <Window.h>
 
 class TagsWindow : public BWindow{
 
 	public:
 	
 						TagsWindow();
 		virtual void	MessageReceived(BMessage *message);	
 
 	private:
 	
 		BButton		*fDoneButton;
 		BButton		*fUndoButton;
 	
 
 };
 
 
 #endif