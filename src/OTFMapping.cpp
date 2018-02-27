//============================================================================
// Name        : OTFMapping.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : OTF Mapping for Tid-70M
//============================================================================

#include <iostream>
using namespace std;
#include "SkyCoord.h"
#include "EarthLocation.h"
#include "Telescope.h"
#include "Scan.h"
#include <iostream>
#include <fstream>

int main() {
	EarthLocation earth(-35.402, 148.981, "2017:12:27:21:28:30");
	double RA = 170.0;
	double DEC = -35.0;
	Scan scan1(RA, DEC);
	int secsToSkyView = 0;
	for (int i = 0; i < scan1.numberOfStepsRA * scan1.numberOfStepsDEC; i++) {
		if (scan1.raster(scan1.RAPath[i], scan1.DECPath[i], earth)) {
			scan1.incrementTimeArray(earth, 21);
			secsToSkyView = +20;
		} else {
			scan1.incrementTimeArray(earth, 13);
			secsToSkyView = +10;
		}
		if (secsToSkyView > 30 * 60) {
			scan1.incrementTimeArray(earth, 15 * 60);
			secsToSkyView = 0;
		}
	}
	for (int i = 0; i < scan1.numberOfStepsRA; i++) {
		cout << setw(3) << i << ") ";
		for (int j = 0; j < scan1.numberOfStepsDEC; j++) {
			cout << scan1.map[j][i] << " ";
		}
		cout << "\n";
	}
	cout << "\nSkipped: " << scan1.skipped << "/"
			<< scan1.numberOfStepsRA * scan1.numberOfStepsDEC;
	cout << "RAs: " << scan1.numberOfStepsRA << " DEC: "
			<< scan1.numberOfStepsDEC << "";
	ofstream fout;
	fout.open("/Users/sajitgurubacharya/Desktop/OTF/OTF.csv");
	for (int i = 0; i < scan1.finalAltAZ.size(); i++) {
		//cout <<"\n"<< i<< ") "<< scan1.finalAltAZ[i];
		fout << "\n" << scan1.finalAltAZ[i];
	}
	fout.close();

	return 0;
}

//int main() {
//	EarthLocation earth(-35.402, 148.981, "2017:12:27:21:28:30");
//	double RA = 170.0;
//	double DEC = -35.0;
//	Scan scan1(RA, DEC);
//	scan1.raster(RA, DEC, earth);
//	cout << "\n";
//	EarthLocation earth2(-35.402, 148.981, "2017:12:27:22:28:30");
//	scan1.raster(RA, DEC, earth2);
//	cout << "\n";
//	EarthLocation earth3(-35.402, 148.981, "2017:12:27:23:28:30");
//	scan1.raster(RA, DEC, earth3);
//	cout << "\n";
//	EarthLocation earth4(-35.402, 148.981, "2018:12:27:23:28:30");
//	scan1.raster(RA, DEC, earth4);
//	cout << "\n";
//	return 0;
//}
