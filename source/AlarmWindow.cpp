/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 *
 * Authors:
 *
 *			Ilio Catallo
 *			Stefano Celentano
 *
 * Last revision: Florian Thaler, April 2021
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
#include <TimeFormat.h>
#include <Window.h>

#include <stdlib.h>
#include <stdio.h>

//translation
#include <Catalog.h>
#include <TranslationUtils.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "AlarmWindow"

class BSpinner;

// Messages
#define BUTTON_ALARM_OK 	'alok'
#define BUTTON_ALARM_CANCEL 'btcn'
#define SET_ALARM 			'salr'
#define ALARM_MSG 			'alrm'
#define ALARM_CLOSE			'_alc'
#define MSG_HOUR			'msgh'
#define MSG_MINUTE			'msgm'
#define MSG_DAY			'msgd'
#define MSG_MONTH			'msgo'
#define MSG_YEAR			'msgy'
/*
* Constructor
* It is created with the dimensions of BRect
*/
AlarmWindow :: AlarmWindow (BRect frame, BHandler *handler)
			: BWindow (frame, B_TRANSLATE("Set an alarm for this note"), B_TITLED_WINDOW,B_NOT_RESIZABLE) {

	// Variables
    fMessenger = new BMessenger(handler);

    char	dayNow[3];
    char 	monthNow[4];
    char 	yearNow[5];
    char 	hourNow[6];
	char	minNow[7];

	// Initialize text fields with current system time values
	sprintf(minNow,"%02d", GetTime(0)+3);
	sprintf(dayNow, "%02d", GetTime(2));
	sprintf(hourNow, "%02d", GetTime(1));
	sprintf(monthNow, "%02d", GetTime(3));
	sprintf(yearNow, "%d", GetTime(4));

	//user interface
	fHour = new BSpinner("Hour", B_TRANSLATE("Hour:"), new BMessage(MSG_HOUR));
	fHour->SetRange(0, 24);
	fHour->SetValue(atoi(hourNow));
	fMinute = new BSpinner("Minute", B_TRANSLATE("Minute:"), new BMessage(MSG_MINUTE));
	fMinute->SetRange(0, 60);
	fMinute->SetValue(atoi(minNow));
	fDay = new BSpinner("Day", B_TRANSLATE("Day:"), new BMessage(MSG_DAY));
	fDay->SetRange(0, 31);
	fDay->SetValue(atoi(dayNow));
	fMonth = new BSpinner("Month", B_TRANSLATE("Month:"), new BMessage(MSG_MONTH));
	fMonth->SetRange(0, 12);
	fMonth->SetValue(atoi(monthNow));
	fYear = new BSpinner("Year", B_TRANSLATE("Year:"), new BMessage(MSG_YEAR));
	fYear->SetValue(atoi(yearNow));

	fButtonOk = new BButton ("ok", B_TRANSLATE("OK"), new BMessage(BUTTON_ALARM_OK));
	fButtonCancel = new BButton ("cancel",B_TRANSLATE("Cancel"),new BMessage(BUTTON_ALARM_CANCEL));

	SetLayout(new BGroupLayout(B_VERTICAL));
	BView* fTopView = new BGroupView(B_VERTICAL);

	BLayoutBuilder::Group<>(fTopView, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGrid()
			.Add(fDay, 1, 0)
			.Add(fMonth, 2, 0)
			.Add(fYear, 3, 0)
			.Add(fHour, 1, 1)
			.Add(fMinute, 2, 1)
		.End()
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(fButtonCancel)
			.Add(fButtonOk)
	.End();
	AddChild(fTopView);
	Show();
}

// Receiving the messages
void AlarmWindow :: MessageReceived(BMessage* message) {
	// Variables
	BMessage 	*msg;
	int32		hourN,minuteN,dayN,monthN,yearN;
	int32 		daysInMonth;

	switch (message->what) {

		case BUTTON_ALARM_OK: {
			/*
			 * When I press OK I throw the message that fills the struct
			 * We prepare the data to be included in the message
			 */

			hourN = fHour->Value();
			minuteN = fMinute->Value();
			dayN = fDay->Value();
			monthN = fMonth->Value();
			yearN = fYear->Value();


			/*
			* Starting controls for time and date
			* Notice: I won't send ALARM_MSG if one of these checks fails
			*/

			// First check if there are any values (in a correct range)
			if ( (hourN > 0 && hourN < 24)  && (minuteN > 0 && minuteN < 60) && (dayN > 0) && (monthN > 0  && monthN <= 12) && (yearN >= 1970 && yearN <= 2150) ) {

				// Second check if day is correct in its month
				daysInMonth = GetDaysInMonth(monthN,yearN);

				if(dayN > daysInMonth) {

					BAlert *myAlert = new BAlert(B_TRANSLATE("Incorrect day in month"), B_TRANSLATE("Insert a correct day for selected month"), B_TRANSLATE("OK"), NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
					myAlert->Go();
					break;
				}

				// Third check if input user date/time comes after system time
				if ( !(IsAfter(minuteN,hourN,dayN,monthN,yearN)) ) {

					BAlert *myAlert = new BAlert(B_TRANSLATE("Previous date-time"), B_TRANSLATE("Date-time should come after system time"), B_TRANSLATE("OK"), NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
					myAlert->Go();
					break;

				}

				// Instantiate a new message and fill it with time values
				msg = new BMessage (ALARM_MSG);

				msg->AddInt16 ("hour", hourN);
				msg->AddInt16 ("minute", minuteN);
				msg->AddInt16 ("day", dayN);
				msg->AddInt16 ("month", monthN);
				msg->AddInt16 ("year", yearN);

				// Sending the message
				fMessenger->SendMessage(msg);

				// Closing the window
				Quit();


			} else {

				// If some values are missing show an alert
				BAlert *myAlert = new BAlert(B_TRANSLATE("Missing values"), B_TRANSLATE("Fill all the fields with correct values"), B_TRANSLATE("OK"), NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
				myAlert->Go();

			}

		}
		break;

		case BUTTON_ALARM_CANCEL:{

			Quit();

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
	timeinfo->tm_year = y - 1900;
	timeinfo->tm_mon = mon - 1;
	timeinfo->tm_mday = d;
	timeinfo->tm_hour = h;
	timeinfo->tm_min = min;
	timeinfo->tm_sec = 0;


	// Convert from struct tm to data type time_t
	userTime = mktime(timeinfo);

	// Compare user time and system time
	if (difftime(userTime, time( &rawtime)) > 0 )
		return true;
	return false;
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
			return now->tm_min;
		case 1:
			return now->tm_hour;
		case 2:
			return now->tm_mday;
		case 3:
			return now->tm_mon + 1;
		case 4:
			return now->tm_year + 1900;
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
