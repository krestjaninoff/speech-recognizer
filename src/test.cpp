#include <iostream>
#include <stdlib.h>
#include "common.h"
#include "math/MFCC.h"

using namespace std;
using namespace wtm::math;

/**
 * Test entry point
 */
int main() {

	/*
	std::vector<raw_t>* f = new std::vector<raw_t>();

	f->push_back((raw_t) 8);
	f->push_back((raw_t) 4);
	f->push_back((raw_t) 8);
	f->push_back((raw_t) 0);

	const double* fourierRaw = MFCC::fourierTransform(*f, 0, 3, true);
	for (int i = 0; i <= 3; i++) {
		cout << fourierRaw[i] << endl;
	}

	cout << endl;
	*/

	double** melFreq = MFCC::getMelFilters(400, 16000);
	for (int m = 0; m <= MFCC_SIZE; m++) {
		for (int i = 0; i <= 400; i++) {
			cout << melFreq[m][i] << endl;
		}
		cout << endl << endl;
	}

	delete [] melFreq;

	return EXIT_SUCCESS;
}
