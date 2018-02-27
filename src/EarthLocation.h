/*
 * EarthLocation.h
 *
 *  Created on: Jan 25, 2018
 *      Author: sajitgurubacharya
 */

#ifndef EARTHLOCATION_H_
#define EARTHLOCATION_H_

class EarthLocation {
	int years, months, days, hours, minutes, seconds;
public:
	double longitude;
	double latitude;
	string timee;
	EarthLocation(double, double, string);
	double getLon();
	double getLat();
	long time();
	void setTime(string time);
	void setTime_(int, int, int, int, int, int);
	string stringTime(int, int, int, int, int, int);
	string getTime();
	EarthLocation() {
	}
	void getTime_(int&, int&, int&, int&, int&, int&);

	EarthLocation operator()(double lat, double lon, string time) {
		EarthLocation E;
		E.latitude = lat;
		E.longitude = lon;
		E.timee = time;
		setTime(timee);
		return E;
	}
};

void EarthLocation::setTime_(int years, int months, int days, int hours,
		int minutes, int seconds) {
	this->years = years;
	this->months = months;
	this->days = days;
	this->hours = hours;
	this->minutes = minutes;
	this->seconds = seconds;
}
void EarthLocation::getTime_(int& years, int& months, int& days, int& hours,
		int& minutes, int& seconds) {
	years = this->years;
	months = this->months;
	days = this->days;
	hours = this->hours;
	minutes = this->minutes;
	seconds = this->seconds;
}
string EarthLocation::getTime() {
	return timee;
}

string EarthLocation::stringTime(int year, int month, int day, int hour,
		int minute, int second) {
	string MO, DAY, HR, MIN, SEC;
	if (month < 10) {
		MO = '0' + to_string(month);
	} else {
		MO = to_string(month);
	}
	if (day < 10) {
		DAY = '0' + to_string(day);
	} else {
		DAY = to_string(day);
	}
	if (hour < 10) {
		HR = '0' + to_string(hour);
	} else {
		HR = to_string(hour);
	}
	if (minute < 10) {
		MIN = '0' + to_string(minute);
	} else {
		MIN = to_string(minute);
	}
	if (second < 10) {
		SEC = '0' + to_string(second);
	} else {
		SEC = to_string(second);
	}

	string tempTime = to_string(year) + '-' + MO + "-"
			+ DAY + " " + HR + ":" + MIN
			+ ":" + SEC;
	return tempTime;
}
void EarthLocation::setTime(string time) {
	years = stoi(time.substr(0, 4));
	months = stoi(time.substr(5, 2));
	days = stoi(time.substr(8, 2));
	hours = stoi(time.substr(11, 2));
	minutes = stoi(time.substr(14, 2));
	seconds = stoi(time.substr(17, 2));
}
long EarthLocation::time() { // for SkyCoord
	return (years * 10000000000 + months * 100000000 + days * 1000000
			+ hours * 10000 + minutes * 100 + seconds);
}

EarthLocation::EarthLocation(double lat, double lon, string time) {
	longitude = lon;
	latitude = lat;
	timee = time;
	setTime(time);
}

double EarthLocation::getLon() {
	return longitude;
}

double EarthLocation::getLat() {
	return latitude;
}

#endif /* EARTHLOCATION_H_ */
