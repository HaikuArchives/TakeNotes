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
 * Description: header of AlarmWindow
 */

#ifndef ALARM_WINDOW_H
#define ALARM_WINDOW_H


#include <Alert.h>
#include <Box.h>
#include <Button.h>
#include <Messenger.h>
#include <StringView.h>
#include <Spinner.h>
#include <TextControl.h>
#include <Window.h>

// Window that allows the user to insert the alarm
class AlarmWindow : public BWindow {

	public:

					 	AlarmWindow(BRect frame, BHandler *handler);
		virtual void 	MessageReceived(BMessage* message);
		virtual bool	QuitRequested();
		virtual void	Quit();

	private:

		BSpinner		*fHour,
						*fMinute,
						*fDay,
						*fMonth,
						*fYear;

		BView 			*fTopView;

		BButton			*fButtonOk;
		BButton			*fButtonCancel;

		BMessenger		*fMessenger;

		int32			GetDaysInMonth(int month, int year);
		bool 			IsAfter(int min, int h, int d, int mon, int y);
		int				GetTime(int element);


};

#endif
