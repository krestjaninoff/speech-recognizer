#include <stdlib.h>
#include <math.h>
#include "MFCC.h"

namespace wtm {
namespace math {

const double* MFCC::transform(const vector<raw_t>& source, length_t start, length_t finish) {

	const double* fourierRaw = fourierTransform(source, start, finish);
	const double* melFreqData = melFreq(fourierRaw, finish - start + 1, MFCC_SIZE);
	const double* logPowerData = logPower(melFreqData, MFCC_SIZE);
	const double* dstTransformData = dstTransform(logPowerData, MFCC_SIZE);

	delete [] melFreqData;
	delete [] logPowerData;
	delete [] fourierRaw;

	return dstTransformData;
}

/**
 * Perform short-time fourier transform with Hamming windows
 */
const double* MFCC::fourierTransform(const vector<raw_t>& source, length_t start, length_t finish) {

	uint32_t size = finish - start + 1;
	double* fourierRaw = new double[size];

	for (length_t k = 0; k < size; k++) {
		fourierRaw[k] = 0;

		for (length_t n = 0; n < MFCC_SIZE; n++) {
			raw_t sample = static_cast<double>(source[start + n]);

			double x = sample * pow(EXP, -2. * PI * k * n / size);
			double w = 1;//0.54 + 0.46 * cos(2 * PI * (size - n) / (size - 1));  // Hamming window
			fourierRaw[k] += x * w;
		}
	}

	return fourierRaw;
}

/**
 * Map the powers of the spectrum obtained above onto the mel scale, using triangular overlapping windows
 */
const double* MFCC::melFreq(const double* fourierRaw, uint32_t size, uint8_t mfccSize) {

	double* melFreq = new double[mfccSize];
	uint8_t M = mfccSize;

	// Creating mel scale on triangular windows
	double* melWindow = new double[size];
	for (length_t m = 0; m < mfccSize; m++) {
		//double mel = 700. * (exp( x / 1125.) - 1.);

		double mel = 1125. * log(1. + m / 700.);
		double triangWindow = 1 - fabs(((M - m) - (M - 1) / 2) / M);
		melWindow[m] = mel * triangWindow;
	}

	// Mapping
	for (length_t m = 0; m < mfccSize; m++) {
		melFreq[m] = 0;

		for (length_t k = 0; k < size; k++) {
			melFreq[m] += fourierRaw[k] * melWindow[m];
		}
	}

	delete [] melWindow;

	return melFreq;
}

/**
 * Take the logs of the powers at each of the mel frequencies
 */
const double* MFCC::logPower(const double* melFreq, uint8_t mfccSize) {

	double* logPower = new double[mfccSize];

	for (length_t k = 0; k < mfccSize; k++) {
		logPower[k] = log(pow(melFreq[k], 2));
	}

	return logPower;
}

const double* MFCC::dstTransform(const double* logPower, uint8_t mfccSize) {

	double* dstTransform = new double[mfccSize];
	//log(pow(melScale[n], 2)

	for (length_t k = 0; k < mfccSize; k++) {
		dstTransform[k] = 0;

		for (length_t n = 0; n < mfccSize; n++) {
			dstTransform[k] += logPower[n] * cos(PI * k * (n + 1./2.) / mfccSize);
		}
	}

	return dstTransform;
}

} /* namespace math */
} /* namespace wtm */
