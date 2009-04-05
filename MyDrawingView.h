#ifndef MY_DRAWING_VIEW_H
#define MY_DRAWING_VIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif

class MyDrawingView : public BView {
	public:
		MyDrawingView (BRect frame, char *name);
		virtual void AttachedToWindow ();
		virtual void Draw(BRect updateRect);
		void setColor(rgb_color colore);
};

#endif