/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Stefano Celentano
 * 
 * Last revision: Stefano Celentano, 19th April 2009
 *
 * Description: TODO
 */


#ifndef ALARM_WINDOW_H
#define ALARM_WINDOW_H


#include "AlarmView.h"

#include <Window.h>
#include <TextControl.h>
#include <Button.h>
#include <Messenger.h>
#include <Alert.h>

// Window that allows the user to insert the alarm
class AlarmWindow : public BWindow {
	
	public:
			
					 AlarmWindow(BRect frame, BHandler *handler);
		virtual void MessageReceived(BMessage* message);
	
	private:
		
		AlarmView 		*fAlarmView;		
		
		BTextControl 	*hour,
						*minute,
						*day,
						*month,
						*year;
		
		BButton			*fButtonOk;
		BMessenger		*fMessenger;
		
		int32			GetDaysInMonth(int month, int year);
		bool 			IsAfter(int min, int h, int d, int mon, int y);

};

#endif