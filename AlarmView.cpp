#ifndef ALARM_VIEW_H
#include "AlarmView.h"
#endif

#ifndef _APPLICATION_H
#include <Application.h>
#endif

AlarmView::AlarmView(BRect rect, char *name) : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW) {

}

void AlarmView :: AttachedToWindow() {
	SetFont (be_bold_font);
	SetFontSize(12);
}

void AlarmView :: Draw (BRect updateRect) {
	MovePenTo(BPoint(20.0, 20.0));
	DrawString ("Inserisci data ed ora per impostare l'allarme:");
}

