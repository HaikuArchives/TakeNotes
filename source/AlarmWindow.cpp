/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Ilio Catallo
 *			Stefano Celentano
 *
 * Last revision: Stefano Celentano, 30th June 2009
 *
 * Description: Alarm Window allows the user to create an alarm for the note
 */

// Our Libraries
#include "AlarmWindow.h"

// Libraries
#include <Alert.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <LayoutBuilder.h>

#include <stdlib.h>
#include <stdio.h>

//translation
#include <Catalog.h>
#include <TranslationUtils.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "AlarmWindow"


// Messages
#define BUTTON_ALARM_OK 	'alok'
#define BUTTON_ALARM_CANCEL 	'btcn'
#define SET_ALARM 			'salr'
#define ALARM_MSG 			'alrm'
#define ALARM_CLOSE			'_alc'

/*
* Constructor
* It is created with the dimensions of BRect
*/
AlarmWindow :: AlarmWindow (BRect frame, BHandler *handler)
			: BWindow (frame, B_TRANSLATE("Set an alarm for this note"), B_TITLED_WINDOW,B_NOT_RESIZABLE) {

	// Variables
    fMessenger = new BMessenger(handler);

    char	dayDefaultField[3];
    char 	monthDefaultField[3];
    char 	yearDefaultField[3];
    char 	minuteDefaultField[3];
    char 	hourDefaultField[3];

	// Initialize text fields with current system time values
	sprintf(minuteDefaultField, "%02d", GetTime(0));
	sprintf(hourDefaultField, "%02d", GetTime(1));
	sprintf(dayDefaultField, "%02d", GetTime(2));
	sprintf(monthDefaultField, "%02d", GetTime(3));
	sprintf(yearDefaultField, "%d", GetTime(4));

	// We allocate the view AlarmView and associate it to the AlarmWindow
	frame.OffsetTo(B_ORIGIN);
	fAlarmView = new AlarmView(frame,"AlarmView");
	fAlarmView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	//AddChild(fAlarmView);

	// Text fields for the data
	hour = new BTextControl(BRect(10,10,100,35),   B_TRANSLATE("hour"),     B_TRANSLATE("hour:"), hourDefaultField , NULL);
	minute = new BTextControl(BRect(110,10,210,35), B_TRANSLATE("minute"),  B_TRANSLATE("min:"), minuteDefaultField, NULL);
	day = new BTextControl(BRect(10,50,100,75),   B_TRANSLATE("day"),  B_TRANSLATE("day:"), dayDefaultField, NULL);
	month = new BTextControl(BRect(110,50,200,75), B_TRANSLATE("month"),    B_TRANSLATE("month:"), monthDefaultField, NULL);
	year = new BTextControl(BRect(210,50,300,75),  B_TRANSLATE("year"),     B_TRANSLATE("year:"), yearDefaultField, NULL);

	// Text field label: visible
	hour -> SetDivider(hour->StringWidth("hour:") + 5);
	minute -> SetDivider(minute->StringWidth("min:") + 5);
	day -> SetDivider(day->StringWidth("day:") + 5);
	month -> SetDivider(month->StringWidth("month:") + 5);
	year -> SetDivider(year->StringWidth("year:") + 5);

	// Allocate the OK button
	fButtonOk = new BButton (BRect(200,110,300,135),"ok", B_TRANSLATE("Ok"), new BMessage(BUTTON_ALARM_OK));
	fButtonCancel = new BButton (BRect(320,110,420,135),"cancel",B_TRANSLATE("Cancel"),new BMessage(BUTTON_ALARM_CANCEL));

	//BLayout
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(B_USE_DEFAULT_SPACING)
			.Add(hour)
			.Add(minute)
			.AddGlue()
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(day)
			.Add(month)
			.Add(year)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fButtonOk)
			.Add(fButtonCancel)
			.Add(fAlarmView)
		.End();
	// Making all the objects part of the view
	/*fAlarmView->AddChild(hour);
	fAlarmView->AddChild(minute);
	fAlarmView->AddChild(day);
	fAlarmView->AddChild(month);
	fAlarmView->AddChild(year);*/

	//fAlarmView->AddChild(fButtonOk);
	//fAlarmView->AddChild(fButtonCancel);

	Show();

}

// Receiving the messages
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
			* Get text fields context by calling Text()
			* (returns null if empty) and convert to int
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

			/*
			* Starting controls for time and date
			* Notice: I won't send ALARM_MSG if one of these checks is missed
			*/

			// First check if there are any values (in a correct range)
			if ( (hourN > 0 && hourN < 24)  && (minuteN > 0 && minuteN < 60) && (dayN > 0) && (monthN > 0  && monthN <= 12) && (yearN >= 1970 && yearN <= 2150) ) {

				// Second check if day is correct in its month
				daysInMonth = GetDaysInMonth(monthN,yearN);

				if(dayN > daysInMonth) {

					BAlert *myAlert = new BAlert(B_TRANSLATE("Incorrect day in month"), B_TRANSLATE("Insert a correct day for selected month"), B_TRANSLATE("OK"), NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
					myAlert -> Go();
					break;
				}

				// Third check if input user date/time comes after system time
				if ( !(IsAfter(minuteN,hourN,dayN,monthN,yearN)) ) {

					BAlert *myAlert = new BAlert(B_TRANSLATE("Previous date-time"), B_TRANSLATE("Date-time should come after system time"), B_TRANSLATE("OK"), NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
					myAlert -> Go();
					break;

				}

				// Instantiate a new message and fill it with time values
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

				// If some values are missing show an alert
				BAlert *myAlert = new BAlert(B_TRANSLATE("Missing values"), B_TRANSLATE("Fill all the fields with correct values"), B_TRANSLATE("OK"), NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
				myAlert -> Go();

			}

		}
		break;

		case BUTTON_ALARM_CANCEL:{

			BAlert *alert = new BAlert("", B_TRANSLATE("The alarm hasn't been saved yet, do you really want to close the window?"), B_TRANSLATE("Yes"), B_TRANSLATE("No"), NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->SetShortcut(0, B_ESCAPE);

			if (alert->Go() == 0) {
				// Discard all the changes
				Quit();
			}


		}
		break;

		default:
			BWindow::MessageReceived(message);
	}
}

// Returns days in month
int32 AlarmWindow :: GetDaysInMonth(int month, int year) {

	switch(month) {
		case 0:
			return 0;
		case 1:
			return 31;
		case 2: {
			if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
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

	// Variables
	time_t rawtime;
	time_t userTime;
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
	if ( difftime(userTime, time( &rawtime) ) > 0 ) {
		return true;
	}
	else {
		return false;
	}
}

int AlarmWindow :: GetTime(int element) {

	// Variables
	struct tm *now;
	time_t time_value;

	// Initialize tm struct and time_t value
	now = NULL;
	time_value = 0;

	// Get time value
	time_value = time(NULL);

	// Get time and date structure
	now = localtime(&time_value);

	switch(element) {
		case 0:
			printf("%02d\n",now -> tm_min);
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

void AlarmWindow :: Quit(){

	// Variables
	BMessage *message;

	// Inform NoteWindow that this window is going to be closed
	message = new BMessage (ALARM_CLOSE);
	fMessenger->SendMessage(message);

	// Execute the real code
	BWindow :: Quit();

}


// If quit is requested send an ALARM_CLOSE message
bool AlarmWindow :: QuitRequested () {

	Quit();
	return true;
}
