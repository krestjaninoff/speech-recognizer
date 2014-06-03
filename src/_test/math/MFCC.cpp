#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include <MFCC.h>
#include <cstring>

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
	uint8_t mfccCnt = 5;
	uint32_t filterLength = 8;
	uint32_t frequency = 8000;
	uint32_t freqMin = 300;
	uint32_t freqMax = 3000;
	double** melFilter = MFCC::getMelFilters(mfccCnt, filterLength, frequency, freqMin, freqMax);

	double* melFilterEx = new double[mfccCnt * filterLength];
	memset((void*) melFilterEx, 0, sizeof(double) * mfccCnt * filterLength);
	melFilterEx[6] = 0.90835;
	melFilterEx[7] = 0.09165;
	melFilterEx[12] = 0.21409;
	melFilterEx[13] = 0.78591;
	melFilterEx[19] = 0.96214;
	melFilterEx[20] = 0.03786;
	melFilterEx[25] = 0.98536;
	melFilterEx[30] = 0.19707;

	// Just check if code runs without exceptions
	for (uint32_t i = 0; i <= mfccCnt; i++) {
		for (uint32_t j = 0; j <= filterLength; j++) {
			EXPECT_NEAR(melFilterEx[i * filterLength + j], melFilter[i][j],
					numeric_limits<double>::epsilon());
		}
	}

	delete [] melFilterEx;
	for (int m = 0; m <= MFCC_SIZE; m++) {
		delete [] melFilter[m];
	}
	delete [] melFilter;
}

