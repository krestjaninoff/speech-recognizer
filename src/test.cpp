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

	std::vector<raw_t>* f = new std::vector<raw_t>();

	f->push_back((raw_t) 1);
	f->push_back((raw_t) 2);
	f->push_back((raw_t) 3);
	f->push_back((raw_t) 2);
	f->push_back((raw_t) 1);
	f->push_back((raw_t) 5);
	f->push_back((raw_t) 3);
	f->push_back((raw_t) 4);
	f->push_back((raw_t) 2);
	f->push_back((raw_t) 3);

	int val = f->at(1);
	cout << val << endl;

	/*
	const double* mfcc = MFCC::transform(*f, 0, 9);

	for (int i = 0; i < 10; i++) {
		cout << mfcc[i] << endl;
	}

	delete [] mfcc;
	*/

	return EXIT_SUCCESS;
}
