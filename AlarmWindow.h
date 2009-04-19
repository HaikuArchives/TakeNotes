#ifndef ALARM_WINDOW_H
#define ALARM_WINDOW_H

#ifndef _WINDOW_H
#include <Window.h>
#endif

#ifndef ALARM_VIEW_H
#include "AlarmView.h"
#endif

#ifndef _CONTROL_H
#include <Control.h>
#endif

class AlarmWindow : public BWindow {
	public:
		AlarmWindow(BRect frame, BHandler *handler);
		virtual void MessageReceived(BMessage* message);
	private:
		AlarmView *fAlarmView;		
		BTextControl *hour,*minute,*day,*month,*year;
		BButton	*fButtonOk;
		
		BMessenger		*fMessenger;

};

#endif