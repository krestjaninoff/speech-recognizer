#include "../math/MFCC.h"
#include "gtest/gtest.h"

using namespace yazz::math;

TEST(MATH_MFCC, FOURIER_TRANSFORM)
{
	std::vector<raw_t>* f = new std::vector<raw_t>();

	f->push_back((raw_t) 8);
	f->push_back((raw_t) 4);
	f->push_back((raw_t) 8);
	f->push_back((raw_t) 0);

	const double* fourierRaw = MFCC::fourierTransform(*f, 0, 3, false);

	const double eps = 0.001;
	EXPECT_NEAR(20, fourierRaw[0], eps);
	EXPECT_NEAR(4, fourierRaw[1], eps);
	EXPECT_NEAR(12, fourierRaw[2], eps);
	EXPECT_NEAR(4, fourierRaw[3], eps);

	delete [] fourierRaw;
}

TEST(MATH_MFCC, MEL_FILTERS)
{
	double** melFreq = MFCC::getMelFilters(400, 16000);

	// Just check if code runs without exceptions
	/*
	for (int m = 0; m <= MFCC_SIZE; m++) {
		for (int i = 0; i <= 400; i++) {
		}
	}
	*/

	for (int m = 0; m <= MFCC_SIZE; m++) {
		delete [] melFreq[m];
	}
	delete [] melFreq;
}

