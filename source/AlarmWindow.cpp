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
#include <TimeFormat.h>

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
    char 	monthDefaultField[4];
    char 	yearDefaultField[5];
    char 	hourNow[6];
	char	minNow[7];

	// Initialize text fields with current system time values
	sprintf(minNow,"%02d", GetTime(0)+3);
	sprintf(dayDefaultField, "%02d", GetTime(2));
	sprintf(hourNow, "%02d", GetTime(1));
	sprintf(monthDefaultField, "%02d", GetTime(3));
	sprintf(yearDefaultField, "%d", GetTime(4));

	// We allocate the view AlarmView and associate it to the AlarmWindow
	// Text fields for the data
	hour = 		new BTextControl("hour",    B_TRANSLATE("hour:"), hourNow , NULL);
	minute = 	new BTextControl("min",  B_TRANSLATE("min:"), minNow, NULL);
	day = 		new BTextControl("day",  	B_TRANSLATE("day:"), dayDefaultField, NULL);
	month = 	new BTextControl("month",   B_TRANSLATE("month:"), monthDefaultField, NULL);
	year = 		new BTextControl("year",    B_TRANSLATE("year:"), yearDefaultField, NULL);

	// Allocate the OK button
	fButtonOk = new BButton ("ok", B_TRANSLATE("Ok"), new BMessage(BUTTON_ALARM_OK));
	fButtonCancel = new BButton ("cancel",B_TRANSLATE("Cancel"),new BMessage(BUTTON_ALARM_CANCEL));

	//thaflo, 2021, adding layout management
	SetLayout(new BGroupLayout(B_VERTICAL));

	BView* fTopView = new BGroupView(B_VERTICAL);

	BLayoutBuilder::Group<>(fTopView, B_VERTICAL)
		.SetInsets(5,5,5,5)
		.AddGrid()
			.Add(day, 1, 0)
			.Add(month, 2, 0)
			.Add(year, 3, 0)
			.Add(hour, 1, 1)
			.Add(minute, 2, 1)
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
	int16 		i;
	int32		hourN,minuteN,dayN,monthN,yearN;
	int32 		daysInMonth;

	switch (message->what) {

		case kShowPopUpCalendar:
		{
			int8 which;
			message->FindInt8("which", &which);
			//_ShowPopUpCalendar(which);
			break;
		}

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

			hourTextField = hour->Text();
			minuteTextField = minute->Text();
			dayTextField = day->Text();
			monthTextField = month->Text();
			yearTextField = year->Text();

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

				i = atoi (hourTextField);
				msg->AddInt16 ("hour", i);

				i = atoi (minuteTextField);
				msg->AddInt16 ("minute", i);

				i = atoi (dayTextField);
				msg->AddInt16 ("day", i);

				i = atoi (monthTextField);
				msg->AddInt16 ("month", i);

				i = atoi (yearTextField);
				msg->AddInt16 ("year", i);

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
	bigtime_t time_value;

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
