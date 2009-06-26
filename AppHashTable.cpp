/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Ilio Catallo, 8th June 2009
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
	int found = 0;
	Hash *temp = new Hash();
	
	printf("Mi è arrivata signature '%s' e nota '%s'\n", signature.String(), note.String());
	
	if (head == NULL)
		printf("Guarda che la testa è nulla...\n");
	
	for (temp = head; temp != NULL; temp = temp -> nextHash) {
		printf("Sto analizzando %s\n", (temp -> signature).String());
		if ( (temp -> signature).Compare(signature) == 0)
			break;
	}
	
	int nNotes = temp -> numNotes;
	printf(" - Numero note: %d\n", nNotes);
	
	if ((temp -> notes[nNotes - 1]).Compare(note) == 0) {
		temp -> notes[nNotes - 1] = "";
		temp -> numNotes --;
	}
	else {
	
		for (int i = nNotes - 1; found != 1; i--) {
			printf("Sto analizzando la nota: %s\n", (temp -> notes[i]).String());
			if ((temp -> notes[i - 1]).Compare(note) == 0)
				found = 1;
			printf("Sovrappongo la nota %s con la nota %s\n", (temp -> notes[i - 1]).String(), (temp -> notes[i]).String());
			temp -> notes[i - 1] = temp -> notes[i];
		}
		temp -> notes [nNotes - 1] = "";
		temp -> numNotes --;
	}
	// Non ci sono note rimaste
	if (temp -> numNotes == 0) {
		// Ricerco la signature da cancellare
		for (temp = head; (temp -> nextHash -> signature).Compare(signature) != 0; temp = temp -> nextHash) {
		}
		temp -> nextHash = temp -> nextHash -> nextHash;
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
