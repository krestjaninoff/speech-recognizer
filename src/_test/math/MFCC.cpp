#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include <MFCC.h>
#include <cstring>

using namespace yazz::math;

/**
 * MFCC tests
 *
 * Octave code:
 *
 * frame = [0.12165, -0.05362, -0.18302, -0.25706, -0.28193, -0.26713,-0.22056, -0.15699,-0.10022, -0.07285]
 * frame = frame(:)
 *
 * frameH = frame .* hamming(length(frame))
 * fourier = abs(fft(frame)) # and the same for frameH
 *
 * dst = dst(frame)
 *
 *
 * [fb, freq] = trifbank( 5, 8, [300 3000], 8000, hz2mel, mel2hz )
 * raw=[1.47173, 0.75985, 0.24049, 0.17987, 0.15109, 0.14357, 0.15109, 0.17987]
 */

TEST(MATH_MFCC, FOURIER_TRANSFORM) {

	const uint8_t frameSize = 10;
	const double frame[] = {0.12165, -0.05362, -0.18302, -0.25706, -0.28193, -0.26713,
		-0.22056, -0.15699,-0.10022, -0.07285};

	const double fourierExpected[] = {1.47173, 0.75985, 0.24049, 0.17987, 0.15109, 0.14357,
		0.15109, 0.17987, 0.24049, 0.75985};
	double* fourierResult = MFCC::fourierTransform(frame, 0, frameSize - 1, false);

	for (uint32_t i = 0; i < frameSize; i++) {
		EXPECT_NEAR(fourierExpected[i], fourierResult[i], EPS_TEST);
	}
	delete [] fourierResult;


	const double fourierHamExpected[] = {1.0830011, 0.7111400, 0.1586940, 0.0093373, 0.0068586,
		0.0085555, 0.0068586, 0.0093373, 0.1586940, 0.7111400};
	fourierResult = MFCC::fourierTransform(frame, 0, frameSize - 1, true);

	for (uint32_t i = 0; i < frameSize; i++) {
		EXPECT_NEAR(fourierHamExpected[i], fourierResult[i], EPS_TEST);
	}

	delete [] fourierResult;
}

TEST(MATH_MFCC, MEL_FILTERS) {
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
	for (uint32_t i = 0; i < mfccCnt; i++) {
		for (uint32_t j = 0; j < filterLength; j++) {
			EXPECT_NEAR(melFilterEx[i * filterLength + j], melFilter[i][j], EPS_TEST);
		}
	}

	delete [] melFilterEx;
	for (int m = 0; m <= MFCC_SIZE; m++) {
		delete [] melFilter[m];
	}
	delete [] melFilter;
}

TEST(MATH_MFCC, CALC_POWER) {

	uint8_t mfccCnt = 5;
	uint32_t frameSize = 8;
	uint32_t frequency = 8000;
	uint32_t freqMin = 300;
	uint32_t freqMax = 3000;
	double** melFilter = MFCC::getMelFilters(mfccCnt, frameSize, frequency, freqMin, freqMax);

	const double fourierRaw[] = {1.47173, 0.75985, 0.24049, 0.17987, 0.15109, 0.14357,
		0.15109, 0.17987};
	const double logPowerEx[] = {1.47173, 0.75985, 0.24049, 0.17987, 0.15109};

	double* logPower = MFCC::calcPower(fourierRaw, frameSize, melFilter, mfccCnt);

	// Just check if code runs without exceptions
	for (uint32_t i = 0; i < mfccCnt; i++) {
		EXPECT_NEAR(logPower[i], logPowerEx[i],	EPS_TEST);
	}

	delete [] logPower;
	for (int m = 0; m < mfccCnt; m++) {
		delete [] melFilter[m];
	}
	delete [] melFilter;
}

TEST(MATH_MFCC, DST_TRANSFORM) {

	const uint8_t frameSize = 10;
	const double frame[] = {0.12165, -0.05362, -0.18302, -0.25706, -0.28193, -0.26713,
		-0.22056, -0.15699,-0.10022, -0.07285};

	const double dstExpected[] = {-1.304316, 0.090024, 0.334787, 0.263605, 0.157515, 0.172151,
		0.080532, 0.089974, 0.036045, 0.028483};
	double* dstResult = MFCC::dstTransform(frame, frameSize);

	for (uint32_t i = 0; i < frameSize; i++) {
		EXPECT_NEAR(dstExpected[i], dstResult[i], EPS_TEST);
	}

	delete [] dstResult;
}

