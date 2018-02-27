/*
 * Scan.h
 *
 *  Created on: Jan 25, 2018
 *      Author: sajitgurubacharya
 *
 *
 */

#ifndef SCAN_H_
#define SCAN_H_
#include "SkyCoord.h"
#include "EarthLocation.h"
#include "Date.h"
#include <vector>

class Scan {
	int years, months, days, hours, minutes, seconds;
	int frameHeight;
	int frameWidth;
	double initRA, initDEC;
	int integrationTime;
	int integrationGap;
	double turningLimit;
	double degreeChange;
	double radianChange;
	double scanSeperation;

	int integrationsPerStep;
	double stripeDuration;

	vector<double> timeArray;

public:
	int skipped;
	double getRA();
	double getDEC();
	double beamWidth;
	int numberOfStepsRA;
	int numberOfStepsDEC;
	double* RAPath;
	double* DECPath;
	int** map;
	vector<string> finalAltAZ;

	double stripeSeperation;
	Scan(double, double);
	bool raster(double, double, EarthLocation);
	void incrementTimeArray(EarthLocation&, int);

};

double Scan::getRA() {
	return initRA;
}
double Scan::getDEC() {
	return initDEC;
}
Scan::Scan(double RA, double DEC) {
	skipped = 0;
	years = 0;
	months = 0;
	days = 0;
	hours = 0;
	minutes = 0;
	seconds = 0;
	initRA = RA;
	initDEC = DEC;

	frameHeight = 1;
	frameWidth = 1;
	beamWidth = 50.0 / 3600.0;
	stripeSeperation = beamWidth/2;
	scanSeperation = beamWidth/2;

	integrationTime = 10;
	integrationGap = 10;
	turningLimit = 0.25;
	degreeChange = 0.015;
	radianChange = degreeChange * PI / 180;

	numberOfStepsRA = frameWidth / (scanSeperation);
	numberOfStepsDEC = frameHeight / (stripeSeperation);
	integrationsPerStep = 10;
	stripeDuration = numberOfStepsRA * integrationsPerStep * integrationTime;

	RAPath = new double[numberOfStepsRA * numberOfStepsDEC];
	DECPath = new double[numberOfStepsRA * numberOfStepsDEC];
	for (int i = 0; i < numberOfStepsRA * numberOfStepsDEC; i++) {
		if (i < numberOfStepsRA) {
			DECPath[i] = initDEC;
			RAPath[i] = initRA - ((scanSeperation) * i);
		} else {
			DECPath[i] = initDEC
					- ((stripeSeperation) * (double(i) / numberOfStepsDEC));
			RAPath[i] = RAPath[numberOfStepsRA - (i % numberOfStepsRA)];
		}
	}

	map = new int*[numberOfStepsRA ];
	for (int i = 0; i < numberOfStepsRA ; i++) {
		map[i] = new int[numberOfStepsDEC ];
	}

	for (int i = 0; i < numberOfStepsRA ; i++) {
		for (int j = 0; j < numberOfStepsDEC ; j++) {
			map[i][j] = 0;
		}
	}
}

void Scan::incrementTimeArray(EarthLocation& earth, int timeInc) { // increment time (by 10 or 20), then update as well.

	string time = earth.getTime();
	earth.getTime_(years, months, days, hours, minutes, seconds);
	if (seconds < 60 - timeInc) {
		seconds += timeInc;
	} else if (seconds + timeInc >= 60) {
		if (seconds + timeInc == 60) {
			seconds = 0;
		} else {
			seconds = seconds + timeInc - 60;
		}
		if (minutes < 59) {
			minutes++;
		} else if (minutes == 59) {
			minutes = 0;
			if (hours < 23) {
				hours++;
			} else if (hours == 23) {
				hours = 0;
				Date date(days, months, years);
				date++;
				days = date.getDay();
				months = date.getMonth();
				years = date.getYear();
			}
		}
	}
	earth.setTime_(years, months, days, hours, minutes, seconds);
}
bool Scan::raster(double RA, double DEC, EarthLocation earth) {
	double coord1, coord2;
	double coord1A, coord1B;
	double radian = PI / 180.0;
	double testA, testB;
	SkyCoord A(RA, DEC, earth);
	A.transform(coord1, coord2);
	double coord2A = coord2;
	double coord2B = coord2;
	testA = coord1;
	testB = coord2;
	coord1A = coord1 + 0.015;
	coord1B = coord1 - 0.015;
	A.transformBack(coord1A, coord2A); // AZ ALT to RADEC -> rounding errors!
	A.transformBack(coord1B, coord2B);

	double top = (cos(coord1B * radian) * sin((coord2A - coord2B) * radian));
	top = top * top;
	double temptop = (cos(coord1A * radian) * sin(coord1B * radian))
			- (sin(coord1A * radian) * cos(coord1B * radian)
					* cos((coord2A - coord2B) * radian));
	temptop = temptop * temptop;
	top = top + temptop;
	top = pow(top, 0.5);
	double bottom = sin(coord1A * radian) * sin(coord1B * radian)
			+ cos(coord1A * radian) * cos(coord1B * radian)
					* cos((coord2A - coord2B) * radian);
	double theta = atan2(top, bottom);

	A.transformBack(testA, testB);
	int coord11A = (coord1A - initRA) / (scanSeperation);
	if (coord11A < 0) {
		coord11A *= -1;
	}
	//cout << "\n" << coord11A;
	int coord22A = (coord2A - initDEC) / (stripeSeperation);
	if (coord22A < 0) {
		coord22A *= -1;
	}
	//cout << "\n" << coord22A;
	int coord11B = (coord1B - initRA) / (scanSeperation);
	if (coord11B < 0) {
		coord11B *= -1;
	}
	//cout << "\n" << coord11B;
	int coord22B = (coord2B - initDEC) / (stripeSeperation);
	if (coord22B < 0) {
		coord22B *= -1;
	}
	//	cout << "\n" << coord22B << "\n";
	if (coord11A < numberOfStepsRA && coord11B < numberOfStepsRA
			&& coord22A < numberOfStepsRA && coord22B < numberOfStepsRA) {

		if ((map[coord11A][coord22A] > 0 && map[coord11B][coord22B] > 0)) {
			skipped++;
			cout << "\nVincenty seperation: " << theta * 180 * 3600 / PI;

			cout << "\nSlope: "
					<< atan((coord2A - coord2B / coord1A - coord1B) * radian)
							 << "\n";
			return false;

		} else {
			map[coord11A][coord22A]++;
			map[coord11B][coord22B]++;
			finalAltAZ.push_back(
					to_string(coord1 * PI / 180) + "\n"
							+ to_string(coord2 * PI / 180) + "\n"
							+ earth.stringTime(years, months, days, hours,
									minutes, seconds));
			cout << "\nVincenty seperation: " << theta * 180 * 3600 / PI;

			cout << "\nSlope: "
					<< atan((coord2A - coord2B / coord1A - coord1B) * radian)
							 << "\n";
			return true;
		}

//		if (map[coord11A][coord22A] ==0 || map[coord11B][coord22B] ==0) {
//			map[coord11A][coord22A]++;
//			map[coord11B][coord22B]++;
//			finalAltAZ.push_back(to_string(coord1*PI/180)+"\n"+ to_string(coord2*PI/180)+"\n"+ earth.stringTime(years, months, days, hours, minutes, seconds));
//			return true;
//
//		} else {
//			skipped++;
//			return false;
//
//		}

	} else {
		return true;
	}
}

#endif /* SCAN_H_ */
