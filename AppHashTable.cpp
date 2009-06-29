/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Ilio Catallo, 28th June 2009
 *
 * Description: TODO
 */

// Libraries
#include "AppHashTable.h"

#include <stdlib.h>
#include <stdio.h>

// Constructors
AppHashTable :: AppHashTable () {
	head = NULL;
	tail = NULL;
}

AppHashTable :: AppHashTable (Hash* myHead) {
	head = myHead;
	tail = myHead;
}

// Setter
void AppHashTable :: AddNote (BString mySignature, BString myPath) {
	// This is the first node we will insert
	if (head == NULL)  {
		Hash *newElement = new Hash();
		newElement -> signature = mySignature;
		newElement -> notes[0] = myPath;
		newElement -> numNotes = 1;
		newElement -> nextHash = NULL;
		// Initializing head and tail
		head = newElement;
		tail = newElement;	// It points to the last element
	
	}
	// It is a generic element in the list
	else {
		// See if this signature is now in the list
		Hash *temp = new Hash();
		int found = 0;
		
		for (temp = head; temp != NULL; temp = temp -> nextHash) {
			if ( (temp->signature).Compare(mySignature) == 0){//if (strcmp(temp -> signature, mySignature) == 0) {
				// Signature found
				found = 1;
				break;
			}				
		}
		
		// We insert the element
		// Signature not found: we insert in the tail
		if (found == 0){
			Hash *newElement = new Hash();
			newElement -> signature = mySignature;
			newElement -> notes[0] = myPath;
			newElement -> numNotes = 1;
			newElement -> nextHash = NULL;
			// The tail is pushed to the next element	
			tail -> nextHash = newElement;
			tail = tail -> nextHash;	
		}
		// Signature found: we add there
		else {
			int position = temp -> numNotes;
			
			temp -> notes[position] = myPath;
			temp -> numNotes ++;
			
		
		}
	}
}

// Remove a note from the structure
void AppHashTable :: DeleteNote (BString signature, BString note) {
	// Variables
	int found = 0,
		i = 0,
		nNotes;
	Hash *temp = new Hash();
	
	for (temp = head; temp != NULL; temp = temp -> nextHash) {
		if ( (temp -> signature).Compare(signature) == 0)
			break;
	}
	
	nNotes = temp -> numNotes;

	// There's only a note
	if (nNotes == 1){	
		if (temp->notes[nNotes-1].Compare(note.String()) == 0){
				temp->numNotes = 0;		
		}
		
	} 
	else {
			// It is the first note
			if ((temp -> notes[0]).Compare(note) != 0)
				// Looking for the node that has to be erased
				for (i = 1; i < nNotes && found != 1; i++)
					if ((temp -> notes[i]).Compare(note) == 0) {
						found = i;
						break;
					}
			// Note found: I can erase it
			for (int j = found; j < nNotes; j++) {
				temp -> notes[j] = temp -> notes[j + 1];
			}
			temp -> notes [nNotes - 1] = "";
			temp -> numNotes --;	
	}
	
	// There aren't any other notes in this signature
	if (temp -> numNotes == 0) {
		// I look for the signature that I have to erase		
		if (!head->nextHash){
		
			 head = NULL;
			 return;
			 
		}
		else
			if ((head -> signature).Compare(signature) == 0) {
				// Erase from the head
				head = head -> nextHash;
			}
			else {
				
				for (temp = head; temp->nextHash; temp = temp -> nextHash){	
					if ((temp -> nextHash -> signature).Compare(signature) == 0)
						break;
				}
	
				// We are at the tail?
				if (temp->nextHash->signature.Compare(tail->signature) == 0){
				
					delete temp->nextHash;
					temp->nextHash = NULL;
					
					tail = temp;
					
					return;
				
				}
				
				temp -> nextHash = temp -> nextHash -> nextHash;
			}
	}
}

// Getter
int AppHashTable :: GetNumSignatures() {
	Hash *temp = new Hash();
	int count = 0;
	
	for (temp = head; temp != NULL; temp = temp -> nextHash)
		count ++;
		
	return count;
}

// Number of notes in that signature
int AppHashTable :: GetNumNotes(BString mySignature) {
	Hash *temp = new Hash();
	int count = 0;
	
	for (temp = head; temp != NULL; temp = temp -> nextHash)
		if ( (temp->signature).Compare(mySignature) == 0){ //if (strcmp(temp -> signature, mySignature) == 0) {
			count = temp -> numNotes;
			break;
		}
	
	return count;
}

// Path of the note selected
char* AppHashTable ::GetNote (BString mySignature, int position) {
	Hash *temp = new Hash();
	BString note = NULL;
	
	for (temp = head; temp != NULL; temp = temp -> nextHash)
		if ((temp->signature).Compare(mySignature) == 0){ //if (strcmp (temp -> signature, mySignature) == 0) {
			note = temp -> notes[position];
			break;
		}
		
	return (char *)note.String();
}

// A single signature, selected from the list
char* AppHashTable :: GetSignature (int position) {
	Hash *temp = new Hash();
	
	temp = head;
	for (int i = 0; i < position; i++)
		temp = temp -> nextHash;
		
	return (char *)(temp -> signature).String();
}

bool AppHashTable :: HasElement(){

		if (head) return 1;
		return 0;
}

void AppHashTable :: PrintToStream(){

		printf("(APPHASHTABLE) PrintToStream\n");

		Hash *temp = new Hash();
		temp = head;
		
		for (temp = head;temp != NULL; temp = temp-> nextHash){
		
			printf("(APPHASHTABLE) Nodo: %s\n",temp->signature.String());
			
			for (int i=0;i<temp->numNotes;i++){
			
				printf(">>(APPHASHTABLE) Nota: %s\n",temp->notes[i].String());
			
			}
		
		}
		

}
