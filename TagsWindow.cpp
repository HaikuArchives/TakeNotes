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
 

#include "TagsWindow.h"

#include <TextControl.h>
#include <View.h>

#include <stdio.h>
#include <stdlib.h>

//Constants
#define BUTTON_OK 		'btok'
#define BUTTON_UNDO 	'btun'


TagsWindow :: TagsWindow()
		   : BWindow (BRect(300,300,700,550),"Set Tags for this note",B_TITLED_WINDOW, B_NOT_RESIZABLE){
		   
		//Variables
		BView			*fTagsView;
		
		BTextControl 	*tag1;
		BTextControl 	*tag2;
		BTextControl 	*tag3;
		
		//Create the main view
		
		fTagsView = new BView(Bounds(), "TagsView", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS);
		 
		
		//Create the Text Field 
		tag1 = new BTextControl(BRect(20,40,200,55), "tag1", "First Tag: ", NULL, NULL);
		tag2 = new BTextControl(BRect(20,70,200,85), "tag2", "Second Tag: ", NULL, NULL);
		tag3 = new BTextControl(BRect(20,100,200,135), "tag3", "Third Tag: ", NULL, NULL);

		
		//Add the tag TextControl as children
		AddChild(fTagsView);
		fTagsView -> AddChild(tag1);
		fTagsView -> AddChild(tag2);
		fTagsView -> AddChild(tag3);
		

}

void TagsWindow :: MessageReceived(BMessage *message){

			switch(message->what){
			
				default:
					BWindow::MessageReceived(message);
					break;
					
			}

}