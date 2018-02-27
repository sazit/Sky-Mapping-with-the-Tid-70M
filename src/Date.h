/*
 * Date.h
 *
 *  Created on: 11/08/2014
 *
 */

#ifndef DATE_H_
#define DATE_H_

#include <iostream>
#include <iomanip>

using namespace std;

class Date {
	friend ostream &operator<<(ostream &, const Date &);

private:
	int day;
	int month;
	int year;
	static const int days[]; // array of days per month
	void helpIncrement(); // utility function for incrementing date

public:
	Date(int = 1, int = 1, int = 0);
	int getMonth_();
	void setDate(int, int, int);
	bool leapYear(int) const; // is date in a leap year?
	bool endOfMonth(int) const; // is date at the end of month?
	Date &operator++(); // prefix increment operator
	Date operator++(int); // postfix increment operator
	const Date &operator+=(int); // add days, modify object
	int getDay();
	int getMonth();
	int getYear();

	void showdate();
};

int Date::getDay(){
	return day;
}
int Date::getMonth(){
	return month;
}
int Date::getYear(){
	return year;
}
const int Date::days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

Date::Date(int d, int m, int y) :
		day(d), month(m), year(y) {
//  day = d;
//  month = m;
//  year = y;
	// initialize static member at file scope; one classwide copy
}

// set month, day and year
void Date::setDate(int dd, int mm, int yy) {
	year = yy;
	month = (mm >= 1 && mm <= 12) ? mm : 1;

	// test for a leap year
	if (month == 2 && leapYear(year))
		day = (dd >= 1 && dd <= 29) ? dd : 1;
	else
		day = (dd >= 1 && dd <= days[month]) ? dd : 1;
} // end function setDate

// if the year is a leap year, return true; otherwise, return false
bool Date::leapYear(int testYear) const {
	if (testYear % 400 == 0 || (testYear % 100 != 0 && testYear % 4 == 0))
		return true; // a leap year
	else
		return false; // not a leap year
} // end function leapYear

// determine whether the day is the last day of the month
bool Date::endOfMonth(int testDay) const {
	if (month == 2 && leapYear(year))
		return testDay == 29; // last day of Feb. in leap year
	else
		return testDay == days[month];
} // end function endOfMonth

int Date::getMonth_() {
	if (month > 0 && month < 4) {
		return 1;
	} else if (month > 3 && month < 7) {
		return 2;
	} else if (month > 6 && month < 10) {
		return 3;
	} else if (month > 11 && month < 13) {
		return 4;
	} else {
		return 0;
	}
}
// function to help increment the date
void Date::helpIncrement() {
	// day is not end of month
	if (!endOfMonth(day))
		day++; // increment day
	else if (month < 12) // day is end of month and month < 12
			{
		month++; // increment month
		day = 1; // first day of new month
	} // end if
	else // last day of year
	{
		year++; // increment year
		month = 1; // first month of new year
		day = 1; // first day of new month
	} // end else
} // end function helpIncrement

// overloaded prefix increment operator
Date &Date::operator++() {
	helpIncrement(); // increment date
	return *this; // reference return to create an lvalue
} // end function operator++

// overloaded postfix increment operator; note that the
// dummy integer parameter does not have a parameter name
Date Date::operator++(int) {
	Date temp = *this; // hold current state of object
	helpIncrement();

	// return unincremented, saved, temporary object
	return temp; // value return; not a reference return
} // end function operator++

// add specified number of days to date
const Date &Date::operator+=(int additionalDays) {
	for (int i = 0; i < additionalDays; i++)
		helpIncrement();

	return *this; // enables cascading
} // end function operator+=

// overloaded output operator
ostream &operator<<(ostream &output, const Date &d) {
//   static string monthName[ 13 ] = { "", "January", "February",
//      "March", "April", "May", "June", "July", "August",
//      "September", "October", "November", "December" };
//
//   output << d.day << "  " << monthName[ d.month ] << "  "<< d.year;
	output << setfill('0') << setw(2) << d.day << '/' << setw(2) << d.month
			<< '/' << setw(4) << d.year << "\n";

	return output; // enables cascading
} // end function operator<<

void Date::showdate() {
	cout << "The date is ";
	cout << setfill('0') << setw(2) << day << '/' << setw(2) << month << '/'
			<< setw(2) << year % 100 << endl;
	return;
}

#endif /* DATE_H_ */
