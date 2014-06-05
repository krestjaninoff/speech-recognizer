#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include <MFCC.h>
#include <cstring>

using namespace yazz::math;

/**
 * For all Octave examples above you need to install the octave-signal module!
 */

/**
 * Fourier transformation
 *
 * Octave code:
 * frame = [0.12165, -0.05362, -0.18302, -0.25706, -0.28193, -0.26713,-0.22056, -0.15699,-0.10022, -0.07285]
 * frame = frame(:)
 *
 * frameH = frame .* hamming(length(frame))
 * fourier = abs(fft(frame)) # and the same for frameH
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

/**
 * Mel filter bank for the following arguments:
 *
 * Octave code:
 * %[fb, freq] = trifbank( 6, 100, [300 3000], 4000, hz2mel, mel2hz )
 *
 * X = [...]
 * hold on
 * plot(X(1:100))
 * plot(X(101:200))
 * ...
 * plot(X(501:600))
 * hold off
 */
TEST(MATH_MFCC, MEL_FILTERS) {
	uint8_t mfccCnt = 6;
	uint32_t filterLength = 100;
	uint32_t frequency = 4000;
	uint32_t freqMin = 300;
	uint32_t freqMax = 3000;

	const double melFilterEx[] = {0,0,0,0,0,0,0,0,0.2,0.4,0.6,0.8,1,0.857143,0.714286,0.571429,0.428571,
		0.285714,0.142857,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0.142857,0.285714,0.428571,0.571429,0.714286,0.857143,1,0.857143,0.714286,
		0.571429,0.428571,0.285714,0.142857,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.142857,0.285714,0.428571,0.571429,0.714286,0.857143,1,
		0.888889,0.777778,0.666667,0.555556,0.444444,0.333333,0.222222,0.111111,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.111111,0.222222,0.333333,
		0.444444,0.555556,0.666667,0.777778,0.888889,1,0.909091,0.818182,0.727273,0.636364,0.545455,
		0.454545,0.363636,0.272727,0.181818,0.0909091,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0909091,0.181818,0.272727,0.363636,0.454545,0.545455,
		0.636364,0.727273,0.818182,0.909091,1,0.923077,0.846154,0.769231,0.692308,0.615385,0.538462,
		0.461538,0.384615,0.307692,0.230769,0.153846,0.0769231,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0769231,0.153846,0.230769,0.307692,0.384615,0.461538,
		0.538462,0.615385,0.692308,0.769231,0.846154,0.923077,1,0.9375,0.875,0.8125,0.75,0.6875,0.625,
		0.5625,0.5,0.4375,0.375,0.3125,0.25,0.1875,0.125,0.0625,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0};

	double** melFilters = MFCC::getMelFilters(mfccCnt, filterLength, frequency, freqMin, freqMax);

	for (uint32_t i = 0; i < mfccCnt; i++) {
		for (uint32_t j = 0; j < filterLength; j++) {
			EXPECT_NEAR(melFilterEx[i * filterLength + j], melFilters[i][j], EPS_TEST);
		}
	}

	for (int m = 0; m < mfccCnt; m++) {
		delete [] melFilters[m];
	}
	delete [] melFilters;
}

/**
 * Apply mel filters, calc the logs of the powers
 */
TEST(MATH_MFCC, CALC_POWER) {

	uint8_t mfccCnt = 3;
	uint32_t frameSize = 10;

	double fourierRaw[] = {0.12165, -0.05362, -0.18302, -0.25706, -0.28193, -0.26713,
			-0.22056, -0.15699,-0.10022, -0.07285};

	// TODO Fix me - we need double**
	double filter1[] = {0,0,0.6,0.6,0,0,0,0,0,0};
	double filter2[] = {0,0,0,0.2,0.6,0.6,0.2,0,0,0};
	double filter3[] = {0,0,0,0,0,0.2,0.6,1,0.6,0.2};

	double** melFilter = new double*[3];
	melFilter[0] = filter1;
	melFilter[1] = filter2;
	melFilter[2] = filter3;

	// TODO Check me
	const double logPowerEx[] = {-2.81765, -2.17638, -2.58769};

	double* logPower = MFCC::calcPower(fourierRaw, frameSize, (double**) melFilter, mfccCnt);

	for (uint32_t i = 0; i < mfccCnt; i++) {
		EXPECT_NEAR(logPower[i], logPowerEx[i],	EPS_TEST);
	}

	delete [] logPower;
}

/**
 * Discrete Cosine Transform
 *
 * Octave code:
 *
 * frame=[0.12165, -0.05362, -0.18302, -0.25706, -0.28193, -0.26713, -0.22056, -0.15699,-0.10022, -0.07285]
 * frame=frame(:)
 *
 * dct(frame) * sqrt(length(frame/2))  % the 1st element must be mult on sqrt(length(frame))
 */
TEST(MATH_MFCC, DST_TRANSFORM) {

	const uint8_t frameSize = 10;
	const double frame[] = {0.12165, -0.05362, -0.18302, -0.25706, -0.28193, -0.26713,
		-0.22056, -0.15699, -0.10022, -0.07285};
	const double dctExpected[] = {-1.471730, 0.1963344, 0.7589118, 0.2417663, 0.1304230,
		0.0867125, 0.0434802, 0.0313459, 0.0162630, 0.0087685};

	double* dctResult = MFCC::dctTransform(frame, frameSize);

	for (uint32_t i = 0; i < frameSize; i++) {
		EXPECT_NEAR(dctExpected[i], dctResult[i], EPS_TEST);
	}

	delete [] dctResult;
}

