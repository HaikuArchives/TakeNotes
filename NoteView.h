#ifndef NOTE_VIEW_H
#define NOTE_VIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif

#ifndef _TEXT_VIEW_H
#include <TextView.h>
#endif


class NoteView : public BTextView {
	public:
		NoteView (BRect frame, BRect frameText, char *name,BHandler *handler);
		void SetBackgroundColor(rgb_color colore);
virtual	void InsertText(const char *text, int32 length, int32 offset, const text_run_array *runs=NULL);

	private:
	
		BHandler *handler;
		BMessenger *fMessenger;

};

#endif