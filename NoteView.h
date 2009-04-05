#ifndef NOTE_VIEW_H
#define NOTE_VIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif

class NoteView : public BView {
	public:
		NoteView (BRect frame, char *name);
		virtual void AttachedToWindow ();
		virtual void Draw(BRect updateRect);
		void setColor(rgb_color colore);
};

#endif