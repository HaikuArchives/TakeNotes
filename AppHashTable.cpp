#include "AppHashTable.h"
#include <stdlib.h>
#include <stdio.h>

// Costruttori
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
	// E' la prima cella da inserire
	if (head == NULL)  {
		Hash *newElement = new Hash();
		newElement -> signature = mySignature;
		newElement -> notes[0] = myPath;
		newElement -> numNotes = 1;
		newElement -> nextHash = NULL;
		// Inizializzo testa e coda
		head = newElement;
		tail = newElement;	// Punta all'ultimo elemento nella lista
	}
	// E' un elemento generico della lista
	else {
		// Verifico che questa signature non ci sia già
		Hash *temp = new Hash();
		int found = 0;
		
		for (temp = head; temp != NULL; temp = temp -> nextHash) {
			if ( (temp->signature).Compare(mySignature) == 0){//if (strcmp(temp -> signature, mySignature) == 0) {
				// Signature trovata
				found = 1;
				break;
			}				
		}
		
		// Inserisco l'elemento
		// Signature non trovata: aggiungo al fondo
		if (found == 0){
			Hash *newElement = new Hash();
			newElement -> signature = mySignature;
			newElement -> notes[0] = myPath;
			newElement -> numNotes = 1;
			newElement -> nextHash = NULL;
			// Sposto la coda	
			tail -> nextHash = newElement;
			tail = tail -> nextHash;	
		}
		// Signature trovata: aggiungo lì
		else {
			int position = temp -> numNotes;
			//temp -> notes[position] = NULL;
			temp -> notes[position] = myPath;
			temp -> numNotes ++;
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

char* AppHashTable :: GetSignature (int position) {
	Hash *temp = new Hash();
	
	temp = head;
	for (int i = 0; i < position; i++)
		temp = temp -> nextHash;
		
	return (char *)(temp -> signature).String();
}
