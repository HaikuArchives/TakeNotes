#ifndef NOTE_VIEW_H
#define NOTE_VIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif

//new
#ifndef _TEXT_VIEW_H
#include <TextView.h>
#endif



class NoteView : public BView {
	public:
		NoteView (BRect frame, char *name);
		virtual void AttachedToWindow ();
		virtual void Draw(BRect updateRect);
		void SetColor(rgb_color colore);
		
	//new
	private:
		BTextView		*fTextView;
};

#endif