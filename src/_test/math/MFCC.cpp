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
	double* fourierResult = MFCC::fourierTransform(frame, 0, frameSize, false);

	for (uint32_t i = 0; i < frameSize; i++) {
		EXPECT_NEAR(fourierExpected[i], fourierResult[i], EPS_TEST);
	}
	delete [] fourierResult;


	const double fourierHamExpected[] = {1.0830011, 0.7111400, 0.1586940, 0.0093373, 0.0068586,
		0.0085555, 0.0068586, 0.0093373, 0.1586940, 0.7111400};
	fourierResult = MFCC::fourierTransform(frame, 0, frameSize, true);

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
	uint32_t frameSize = 100;
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

	double** melFilters = MFCC::getMelFilters(mfccCnt, frameSize, frequency, freqMin, freqMax);

	for (uint32_t i = 0; i < mfccCnt; i++) {
		for (uint32_t j = 0; j < frameSize; j++) {
			EXPECT_NEAR(melFilterEx[i * frameSize + j], melFilters[i][j], EPS_TEST);
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

	double filter1[] = {0,0,0.6,0.6,0,0,0,0,0,0};
	double filter2[] = {0,0,0,0.2,0.6,0.6,0.2,0,0,0};
	double filter3[] = {0,0,0,0,0,0.2,0.6,1,0.6,0.2};

	double** melFilter = new double*[3];
	melFilter[0] = filter1;
	melFilter[1] = filter2;
	melFilter[2] = filter3;

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

TEST(MATH_MFCC, CONSOLIDATED) {

	uint8_t mfccCnt = 6;
	uint32_t frameSize = 100;
	uint32_t frequency = 4000;
	uint32_t freqMin = 300;
	uint32_t freqMax = 3000;

	const double frame[] = {0.0937223,	0.172552, 0.242836, 0.286996, 0.308481, 0.309488, 0.290597,
		0.256691, 0.195379, 0.102237, 0.00936918, -0.0420545, -0.0234077, 0.0739769, 0.224769,
		0.398755, 0.578021, 0.731346, 0.83929, 0.903195, 0.90582, 0.827815, 0.679403, 0.490005,
		0.311167, 0.190588, 0.132481, 0.134434, 0.197516, 0.298441, 0.419904, 0.55446, 0.670766,
		0.7528, 0.800378, 0.790857, 0.710959, 0.592395, 0.47029, 0.364513, 0.273415, 0.192755,
		0.142186, 0.131108, 0.136174, 0.153294, 0.192907, 0.239296, 0.268654, 0.264138, 0.221442,
		0.157781, 0.093112, 0.0325022, -0.0160833, -0.0514847, -0.0834376, -0.105777, -0.110019,
		-0.103122, -0.10007, -0.118107, -0.165197, -0.232246, -0.308756, -0.38496, -0.442152,
		-0.47261, -0.485672, -0.484634, -0.459944, -0.409742, -0.355754, -0.320505, -0.305918,
		-0.306223, -0.324534, -0.369427, -0.436659, -0.503189, -0.554918, -0.597156, -0.621754,
		-0.613483, -0.577288, -0.518418, -0.43675, -0.345622, -0.258431, -0.191626, -0.169744,
		-0.194159, -0.242439, -0.300729, -0.36198, -0.425123, -0.480209, -0.493149, -0.449599,
		-0.365917};

	const double mfccExpected[] = {-23.12352, 8.07315, 3.84674, 0.61758, 1.00891, 0.26051};

	double* mfccResult = MFCC::transform(frame, 0, frameSize - 1, mfccCnt, frequency, freqMin, freqMax);

	for (uint8_t i = 0; i < mfccCnt; i++) {
		EXPECT_NEAR(mfccExpected[i], mfccResult[i], EPS_TEST);
	}

	delete [] mfccResult;
}
