// Header file
#include "NoteRefFilter.h"

// System libraries
#include <NodeInfo.h>

#include <string.h>

// Filter hook method
bool
NoteRefFilter :: Filter(const entry_ref* ref, BNode* _node,
			struct stat_beos* stat, const char *_mimeType)
{
	// Follow links
	BEntry entry(ref, true);
	if (entry.InitCheck() != B_OK)
		return false;

	// Return true if the file is a directory
	if (entry.IsDirectory())
		return true;

	// Create a BNode and a BNodeInfo to get the MIME type
	BNode node(&entry);
	if (node.InitCheck() != B_OK)
		return false;
	BNodeInfo nodeInfo(&node);
	if (nodeInfo.InitCheck() != B_OK)
		return false;

	// Create a string to hold the MIME type
	char mimeType[B_MIME_TYPE_LENGTH];

	// Get MIME type
	if (nodeInfo.GetType(mimeType) != B_OK)
		return false;

	// Return true if the MIME type matches
	if (strcmp(mimeType, "application/takenotes") == 0)
		return true;

	// Filter the file out if it doesn't match any of the above
	return false;
}
