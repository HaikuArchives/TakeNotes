#ifndef NOTE_REF_FILTER_H
#define NOTE_REF_FILTER_H

// System libraries
#include <FilePanel.h>

// Definition of the class
class NoteRefFilter : public BRefFilter {

public:
	bool		Filter(const entry_ref* ref, BNode* node, struct stat_beos* stat,
				const char* mimeType);
};

#endif
