/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Stefano Celentano
 * 
 * Last revision: Stefano Celentano, 15th May 2009
 *
 * Description: TODO
 */


#include "AlarmWindow.h"

#include <stdlib.h>
#include <stdio.h>

#define BUTTON_ALARM_OK 'alok'
#define SET_ALARM 		'salr'
#define ALARM_MSG 		'alrm'


AlarmWindow :: AlarmWindow (BRect frame, BHandler *handler) 
			: BWindow (frame, "Set alarm for this note", B_TITLED_WINDOW,B_NOT_RESIZABLE) {
	
    fMessenger = new BMessenger(handler);
    
    char	dayDefaultField[3];
    char 	monthDefaultField[3];
    char 	yearDefaultField[3];
    char 	minuteDefaultField[3];
    char 	hourDefaultField[3];

	sprintf(minuteDefaultField, "%d", GetTime(0));
	sprintf(hourDefaultField, "%d", GetTime(1));
	sprintf(dayDefaultField, "%d", GetTime(2));
	sprintf(monthDefaultField, "%d", GetTime(3));
	sprintf(yearDefaultField, "%d", GetTime(4));

	// We allocate the view AlarmView and associate it to the AlarmWindow	
	frame.OffsetTo(B_ORIGIN);
	fAlarmView = new AlarmView(frame,"AlarmView");
	fAlarmView->SetViewColor(216, 216, 216, 0);
	AddChild(fAlarmView);

	// Text fields for the data
	hour = new BTextControl(BRect(20,40,100,35),   "hour",     "hour:", hourDefaultField , NULL);
	minute = new BTextControl(BRect(120,40,200,35), "minute",  "min:", minuteDefaultField, NULL);
	day = new BTextControl(BRect(20,100,100,35),   "day",     "day:", dayDefaultField, NULL);
	month = new BTextControl(BRect(120,100,200,35), "month",    "month:", monthDefaultField, NULL);
	year = new BTextControl(BRect(220,100,300,35),  "year",     "year:", yearDefaultField, NULL);

	// Text field label: visible
	hour -> SetDivider(hour->StringWidth("hour:") + 5);
	minute -> SetDivider(minute->StringWidth("min:") + 5);
	day -> SetDivider(day->StringWidth("day:") + 5);
	month -> SetDivider(month->StringWidth("month:") + 5);
	year -> SetDivider(year->StringWidth("year:") + 5);

	// Allocate the OK button
	fButtonOk = new BButton (BRect(400,230,450,240),"ok", "OK", new BMessage(BUTTON_ALARM_OK));

	// Making all the objects part of the view	
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
	int32		hourN,minuteN,dayN,monthN,yearN;
	int32 		daysInMonth;

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

			/*
			* Get text fields context by calling Text() (returns null if empty) and convert to int
			*/

			hourTextField = hour -> Text();
			minuteTextField = minute -> Text();
			dayTextField = day -> Text();
			monthTextField = month -> Text();
			yearTextField = year -> Text();
			
			hourN = atoi(hourTextField);
			minuteN = atoi(minuteTextField);
			dayN = atoi(dayTextField);
			monthN = atoi(monthTextField);
			yearN = atoi(yearTextField);

			// Starting controls for time and date
			// Notice: I won't send ALARM_MSG if one of these checks is missed
			
			// First check if there are any values (in a correct range)
			if( (hourN > 0 && hourN < 24)  && (minuteN > 0 && minuteN < 60) && (dayN > 0) && (monthN > 0  && monthN <= 12) && (yearN >= 1970 && yearN <= 2150) ) {			
				
				// Second check if day is correct in its month
				
				daysInMonth = GetDaysInMonth(monthN,yearN);
				
				if(dayN > daysInMonth) {
					
					BAlert *myAlert = new BAlert("Incorrect days in month", "Insert a correct day for selected month", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
					myAlert -> Go();
					break;
				}
				
				// Third check if input user date/time comes after system time
				
				if ( !(IsAfter(minuteN,hourN,dayN,monthN,yearN)) ) {
					
					BAlert *myAlert = new BAlert("Previous date-time", "Date-time should come after system time", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
					myAlert -> Go();
					break;				
				
				}
				
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
			
			
			} else {
			
				BAlert *myAlert = new BAlert("Missing values", "Fill all the fields with correct values", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
				myAlert -> Go();
			
			}	
		
		}
			
		default:			
			BWindow::MessageReceived(message);
	}
}

int32 AlarmWindow :: GetDaysInMonth(int month, int year) {

	switch(month) {
		case 0: 
			return 0;
		case 1: 
			return 31;
		case 2: {
			if ( (year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
				return 29;
			else
				return 28;
		}
		case 3: 
			return 31;
		case 4: 
			return 30;
		case 5: 
			return 31;
		case 6: 
			return 30;
		case 7: 
			return 31;
		case 8: 
			return 31;
		case 9: 
			return 30;
		case 10: 
			return 31;
		case 11: 
			return 30;
		case 12: 
			return 31;
		default: 
			return 31;
	}
}

bool AlarmWindow :: IsAfter(int min, int h, int d, int mon, int y) {


	// Declare two timers
	
	time_t rawtime;
	time_t userTime;
	
	// Declare a time struct
	
	struct tm *timeinfo;
	
	// Get the current time (number of seconds from the "epoch"), stores it in the timer
	
	time( &rawtime );
	
	// Convert time_t time value to a tm struct
	
	timeinfo = localtime ( &rawtime );

		
	// Fill the struct with user input
	
	timeinfo -> tm_year = y - 1900;
	timeinfo -> tm_mon = mon - 1;
	timeinfo -> tm_mday = d;
	timeinfo -> tm_hour = h;
	timeinfo -> tm_min = min;
	timeinfo -> tm_sec = 0;
	
	
	// Convert from struct tm to data type time_t
	
	userTime = mktime(timeinfo);	

	// Compare user time and system time
	
	if( difftime(userTime, time( &rawtime) ) > 0 ) {
		return true;
	}
	else {
		return false;
	}	
}

int AlarmWindow :: GetTime(int element) {


	struct tm *now = NULL;
	time_t time_value = 0;
	
	// Get time value
	time_value = time(NULL);
	
	// Get time and date structure
	now = localtime(&time_value);	

	switch(element) {
		case 0: 
			return now -> tm_min;
		case 1: 
			return now -> tm_hour;
		case 2: 
			return now -> tm_mday;
		case 3: 
			return now -> tm_mon + 1;
		case 4: 
			return now -> tm_year + 1900;
		default:
			return 0;
			
	}	
}