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


#include "AlarmWindow.h"

#include <stdlib.h>

#define BUTTON_ALARM_OK 'alok'
#define SET_ALARM 		'salr'
#define ALARM_MSG 		'alrm'

AlarmWindow :: AlarmWindow (BRect frame, BHandler *handler) 
			: BWindow (frame, "Set alarm for this note", B_TITLED_WINDOW,B_NOT_RESIZABLE) {
	
    fMessenger = new BMessenger(handler);

	// We allocate the view AlarmView and associate it to the AlarmWindow	
	frame.OffsetTo(B_ORIGIN);
	fAlarmView = new AlarmView(frame,"AlarmView");
	fAlarmView->SetViewColor(216, 216, 216);
	AddChild(fAlarmView);

	// Text fields for the data
	hour = new BTextControl(BRect(20,30,100,35),   "hour",     "hour:", "", NULL);
	minute = new BTextControl(BRect(20,70,100,35), "minute",  "min:", "", NULL);
	day = new BTextControl(BRect(20,110,100,35),   "day",     "day:", "", NULL);
	month = new BTextControl(BRect(20,150,100,35), "month",    "month:", "", NULL);
	year = new BTextControl(BRect(20,190,100,35),  "year",     "year:", "", NULL);

	// Text field label: visible
	hour -> SetDivider(hour->StringWidth("hour:") + 5);
	minute -> SetDivider(minute->StringWidth("min:") + 5);
	day -> SetDivider(day->StringWidth("day:") + 5);
	month -> SetDivider(month->StringWidth("month:") + 5);
	year -> SetDivider(year->StringWidth("year:") + 5);

	// We allocate the ok button
	fButtonOk = new BButton (BRect(400,230,450,240),"ok", "OK", new BMessage(BUTTON_ALARM_OK));

	// Macking all the objects part of the view	
	fAlarmView->AddChild(hour);
	fAlarmView->AddChild(minute);
	fAlarmView->AddChild(day);
	fAlarmView->AddChild(month);
	fAlarmView->AddChild(year);

	fAlarmView->AddChild(fButtonOk);
		
	Show();
}

// Receiving the messages...
void AlarmWindow :: MessageReceived(BMessage* message) {
	// Variables
	BMessage 	*msg;
	int16 		i;

	switch (message -> what) {

		case BUTTON_ALARM_OK: {		
			/*
			 * When I press OK I throw the message that fills the struct
			 * We prepare the data to be included in the message	
			 */
			const char *hourTextField;
			const char *minuteTextField;
			const char *dayTextField;
			const char *monthTextField;
			const char *yearTextField;

			hourTextField = hour -> Text();
			minuteTextField = minute -> Text();
			dayTextField = day -> Text();
			monthTextField = month -> Text();
			yearTextField = year -> Text();

			// ToDo: controls!

			msg = new BMessage (ALARM_MSG);
		
			i = atoi (hourTextField);
			msg -> AddInt16 ("hour", i);
		
			i = atoi (minuteTextField);
			msg -> AddInt16 ("minute", i);
		
			i = atoi (dayTextField);
			msg -> AddInt16 ("day", i);
		
			i = atoi (monthTextField);
			msg -> AddInt16 ("month", i);
		
			i = atoi (yearTextField);
			msg -> AddInt16 ("year", i);
		
			// Sending the message	
			fMessenger->SendMessage(msg);

			// Closing the window		
			Quit();			
		
		}
			
		default:			
			BWindow::MessageReceived(message);
	}
}