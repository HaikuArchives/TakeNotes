#ifndef APP_HASH_TABLE_H
#define APP_HASH_TABLE_H

#include <String.h>

// Struttura
/*struct Hash {
	char	*signature;
	char	*notes[20];
	int		numNotes;
	Hash	*nextHash;
};*/

struct Hash {
	BString	signature;
	BString	notes[20];
	int		numNotes;
	Hash	*nextHash;
};
// Definizione della classe
class AppHashTable {
	public:
						AppHashTable();
						AppHashTable(Hash*);
				
				void 	AddNote(BString, BString); //AddNote(char*,char*);
				
				char* 	GetNote(BString, int); //GetNote(char*, int);
				char*	GetSignature (int);
				int		GetNumNotes(BString); //GetNumNotes(char*);
				int		GetNumSignatures();
				
	private:
		Hash*	head;
		Hash*	tail;
};

#endif
