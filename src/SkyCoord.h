/*
 * SkyCoord.h
 *
 *  Created on: Jan 25, 2018
 *      Author: sajitgurubacharya
 *
 *      *double values of time is in local siderial hours!
 *      		currently all time UT, so offset required
 *      		time in condensed format HH:MM:SS -> HHMMSS!
 *      *angles to be calculated in degrees
 *      *epoch J2000
 */

#ifndef SKYCOORD_H_
#define SKYCOORD_H_
#include "EarthLocation.h"
#include <math.h>
#include <cmath>
#include <iomanip>
#define PI 3.14159265358979323846
using namespace std;

const int daysToYear[13] = { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273,
		304, 334 };
const double daysToEpoch[24] = { -731.5, -366.5, -1.5, 364.5, 729.5, 1094.5,
		1459.5, 1825.5, 2190.5, 2555.5, 2920.5, 3286.5, 3651.5, 4016.5, 4381.5,
		4747.5, 5112.5, 5477.5, 5842.5, 6208.5, 6573.5, 6938.5, 7303.5, 7669.5 };

class SkyCoord {
	double decimalTime(long);
	double localSiderialTime(double, double, double);
	double coordA, coordB;
	long LST;
	double latitude;
	double longitude;
	double hours(long);
public:
	SkyCoord(double, double, EarthLocation);
	void transform(double&, double&);
	void transformBack(double&, double&);
};

void SkyCoord::transformBack(double& coord1, double& coord2) {
	double AZ = coord1 + 180.0; // meenus calculates AZ west from south!
	if (AZ>360.0){
		AZ=AZ-360.0;
	}
	double ALT = coord2;
	double LSA = fmod(
			localSiderialTime(decimalTime(LST), longitude, hours(LST)), 360.0);
	double radian = PI / 180.0;
	double sA = sin(AZ * radian);
	double cA = cos(AZ * radian);
	double sH = sin(ALT * radian);
	double cH = cos(ALT * radian);
	double sL = sin(latitude * radian);
	double cL = cos(latitude * radian);
	double H = atan2(sA, (cA * sL) + ((sH / cH) * cL));
	coord1 = (LSA * radian - (-longitude * radian) - (longitude * radian) - H)
			* 180.0 / PI;
	if (coord1>360.0){
		coord1-=360.0;
	}
	/*
	 * tricky because LSA is supposed to be in GMT, but since time zones
	 * and longitudes don't match exactly, i've essentially set longitudes as
	 * time zones(west positive, east negative, default '- (west positive LONGITUDE)'.
	 */
	coord2 = asin((sL * sH) - (cL * cH * cA)) * 180.0 / PI;
	if (coord2>360.0){
		coord2-=360.0;
	}
	cout << "\nConverting Back: RA: " << coord1 << " DEC: " << coord2;
}

void SkyCoord::transform(double& coord1, double& coord2) {
	double RA = coordA;
	double DEC = coordB;
	double radian = PI / 180.0;
	double LSA = fmod(
			localSiderialTime(decimalTime(LST), longitude, hours(LST)), 360);
	double HA = LSA - RA;
	if (HA<0.0){
		HA+=360.0;
	}
	double sinALT = (sin(DEC * PI / 180) * sin(latitude * PI / 180))
			+ (cos(DEC * PI / 180) * cos(latitude * PI / 180)
					* cos(HA * PI / 180));
	double altitude = asin(sinALT);

	double cosAZ = (sin(DEC * PI / 180)
			- (sin(altitude) * sin(latitude * PI / 180)))
			/ (cos(altitude) * cos(latitude * PI / 180));
	double azimuth = (acos(cosAZ)) * 180 / PI;
	if (sin(HA*radian) > 0) {
		azimuth = 360.0 - azimuth;
	}
	altitude = altitude * 180 / PI;
	cout << "\nRA: " << coordA << " DEC: " << coordB;
	cout << "\nAZ: " << setprecision(20) << azimuth << " Altitude: " << altitude
			<< setw(3) << " ";

	coord1 = azimuth;
	coord2 = altitude;

}
SkyCoord::SkyCoord(double Az, double Alt, EarthLocation EL) {
	coordA = Az;
	coordB = Alt;
	LST = EL.time();
	latitude = EL.getLat();
	longitude = EL.getLon();
}

double SkyCoord::localSiderialTime(double daysToEp, double lon, double hours) {
	return (100.46 + (0.985647 * daysToEp) + lon + (15 * hours));
}

double SkyCoord::hours(long time) {
	int k, m, n;
	double k_d;
	k = (time % 1000000) / 10000;
	m = (time % 10000) / 100;
	n = time % 100;
	k_d = k + (m / 60.0) + (n / 3600.0);
	return k_d;
}
double SkyCoord::decimalTime(long time) {
	int year, months, days;
	int hours, minutes, seconds;
	year = time / 10000000000;
	months = (time % 10000000000) / 100000000;
	days = (time % 100000000) / 1000000;
	hours = (time % 1000000) / 10000;
	minutes = (time % 10000) / 100;
	seconds = time % 100;
	cout << year << "-" << months << "-" << days << "  " << hours << ":"
			<< minutes << ":" << seconds << "";
	int daysToYr = daysToYear[months];
	double daysToEp = daysToEpoch[year - 1998];
	double hours_d, minutes_d, seconds_d, daysToYr_d, days_d;
	hours_d = hours;
	minutes_d = minutes;
	seconds_d = seconds;
	days_d = days;
	daysToYr_d = daysToYr;
	double dayPercent = (hours_d + (minutes_d / 60.00) + (seconds_d / 3600.00))
			/ 24.00;
	double answer = (dayPercent + daysToYr_d + days_d + daysToEp);
	return answer;

}

#endif /* SKYCOORD_H_ */
