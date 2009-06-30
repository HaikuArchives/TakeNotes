/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Ilio Catallo
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 8th June 2009
 *
 * Description: Header of the structure
 */

#ifndef APP_HASH_TABLE_H
#define APP_HASH_TABLE_H

// Libraries
#include <String.h>

// Structure of a node
struct Hash {
	BString	signature;
	BString	notes[20];
	int		numNotes;
	Hash	*nextHash;
};
// Class containing the structure with the associations between
// the notes and the applications
class AppHashTable {
	public:
						AppHashTable();
						AppHashTable(Hash*);
				
				void 	AddNote(BString, BString);
				void	DeleteNote (BString, BString);
				
				char* 	GetNote(BString, int); 
				char*	GetSignature (int);
				int		GetNumNotes(BString); 
				int		GetNumSignatures();
				bool	HasElement();
				void	PrintToStream();
				
	private:
		Hash*	head;
		Hash*	tail;
};

#endif
