/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo, Stefano Celentano
 * 
 * Last revision: Stefano Celentano, 3rd June 2009
 *
 * Description: TODO
 */
 

#include "TagsWindow.h"
#include "NoteWindow.h"

#include <View.h>
#include <Node.h>
#include <Alert.h>


#include <stdio.h>
#include <stdlib.h>

//Constants
#define BUTTON_OK 		'btok'
#define BUTTON_UNDO 	'btun'


TagsWindow :: TagsWindow()
		   : BWindow (BRect(300,300,700,550),"Set Tags for this note",B_TITLED_WINDOW, B_NOT_RESIZABLE){
		   
		//Variables
		
		BView			*fTagsView;
		char			buffer_tag1[30];
		char			buffer_tag2[30];
		char			buffer_tag3[30];
									
		//Create the main view
		
		fTagsView = new BView(Bounds(), "TagsView", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS);
		 
		
		//Create the Text Field 
		
		fTag1 = new BTextControl(BRect(20,40,200,55), "tag1", "First Tag: ", NULL, NULL);
		fTag2 = new BTextControl(BRect(20,70,200,85), "tag2", "Second Tag: ", NULL, NULL);
		fTag3 = new BTextControl(BRect(20,100,200,135), "tag3", "Third Tag: ", NULL, NULL);

		//Create the OK and UNDO button
		
		fDoneButton = new BButton(BRect(340,200,390,210), "ok", "OK", new BMessage(BUTTON_OK));
		fUndoButton = new BButton(BRect(270,200,320,210), "undo", "Undo", new BMessage(BUTTON_UNDO));
		
		//Add the tag TextControl as children
		
		AddChild(fTagsView);
		fTagsView -> AddChild(fTag1);
		fTagsView -> AddChild(fTag2);
		fTagsView -> AddChild(fTag3);
		
		//Add OK and UNDO button as children
		
		fTagsView -> AddChild(fDoneButton);
		fTagsView -> AddChild(fUndoButton);
		
		//Initialize text field to old values

		BNode node = BNode(&ref_tags);
			if(node.InitCheck() != B_OK) {
				printf("The object wasn't correctly inizialized\n");
		}	
		
		node.ReadAttr("TAKENOTES:tagone", B_STRING_TYPE, 0, &buffer_tag1, 30);
		node.ReadAttr("TAKENOTES:tagtwo", B_STRING_TYPE, 0, &buffer_tag2, 30);
		node.ReadAttr("TAKENOTES:tagthree", B_STRING_TYPE, 0, &buffer_tag3, 30);

		printf(buffer_tag1);
		printf("\n");
			
		fTag1 -> SetText(buffer_tag1);
		fTag2 -> SetText(buffer_tag2);
		fTag3 -> SetText(buffer_tag3);		

}

void TagsWindow :: MessageReceived(BMessage *message){

			switch(message->what){
				case BUTTON_OK: {
				
						//Variables
						
				
						//Obtain the BNode object (I need to obtain the generic entry_ref, I still don't know how!)
				
						BNode node = BNode(&ref_tags);
						if(node.InitCheck() != B_OK) {
							printf("The object wasn't correctly inizialized\n");
						}
						
						
		
						//Print all the attribute associated with the BNode object (only for debugging)
					
						
						char buffer[60];
		
						while(node.GetNextAttrName(buffer) == B_OK) {
							printf(">Attr name: %s \n", buffer);
						}
																	
						//Set each attribute to text field's content (NOTICE: add some check, like empty check field)
												
						
						if(node.WriteAttr("TAKENOTES:tagone", B_STRING_TYPE, 0, fTag1 -> Text(), 20) == 0) {
							printf("no bytes written for tag one\n");
						}
								
						if(node.WriteAttr("TAKENOTES:tagtwo", B_STRING_TYPE, 0, fTag2 -> Text(), 20) == 0) {
							printf("no bytes written for tag two\n");
						}
								
						if(node.WriteAttr("TAKENOTES:tagthree", B_STRING_TYPE, 0, fTag3 -> Text(), 20) == 0) {
							printf("no bytes written for tag three\n");
						}													
						
						//Print ALL the attributes (only for debugging)
						
						char buffer2[60];
						
						node.ReadAttr("TAKENOTES:tagone", B_STRING_TYPE, 0, &buffer2, 30);
						printf("First tag is: ");
						printf(buffer2);
						printf("\n");
						
						node.ReadAttr("TAKENOTES:tagtwo", B_STRING_TYPE, 0, &buffer2, 30);
						printf("Second tag is: ");
						printf(buffer2);
						printf("\n");
						
						node.ReadAttr("TAKENOTES:tagthree", B_STRING_TYPE, 0, &buffer2, 30);
						printf("Third tag is: ");
						printf(buffer2);
						printf("\n");	
			
						//Closing window
						
						Quit();		
	
																											
				}
				break;
				
				case BUTTON_UNDO: {
				
						//Variables
						
						BAlert *myAlert;
								
						BNode node = BNode(&ref_tags);
						if(node.InitCheck() != B_OK) {
							printf("The object wasn't correctly inizialized\n");
						}
				
						node.WriteAttr("TAKENOTES:tagone", B_STRING_TYPE, 0, NULL, 30);
						node.WriteAttr("TAKENOTES:tagtwo", B_STRING_TYPE, 0, NULL, 30);
						node.WriteAttr("TAKENOTES:tagthree", B_STRING_TYPE, 0, NULL, 30);
				
						fTag1 -> SetText("");
						fTag2 -> SetText("");
						fTag3 -> SetText("");
	
						myAlert = new BAlert("Undo tags", "Tags deleted!", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
						myAlert -> Go();

				}
				break;
			
				default:
					BWindow::MessageReceived(message);
					break;
					
			}

}