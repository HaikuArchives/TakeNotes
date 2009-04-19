#ifndef ALARM_VIEW_H
#define ALARM_VIEW_H

#ifndef _VIEW_H
#include <View.h>
#endif

#ifndef _INTERFACE_H
#include <InterfaceKit.h>
#endif

class AlarmView : public BView {
	public:
		AlarmView (BRect frame, char *name);
		virtual void AttachedToWindow ();
		virtual void Draw(BRect updateRect);
};

#endif