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
		NoteView (BRect frame, BRect frameText, char *name);
		void SetBackgroundColor(rgb_color colore);

};

#endif