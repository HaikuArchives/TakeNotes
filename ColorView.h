#ifndef COLOR_VIEW_H
#define COLOR_VIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

#ifndef _BUTTON_H
#include <Button.h>
#endif

class ColorView : public BView {
	public:
		ColorView (BRect frame, char *name);
		virtual void AttachedToWindow ();
		virtual void Draw(BRect updateRect);
		virtual void MouseDown(BPoint point);
	private:
		BColorControl *fColorControl;
		BButton * fButton;
};

#endif