#include "../common.h"
#include <MFCC.h>
#include <stddef.h>
#include <complex.h>

using namespace std;

namespace yazz {
namespace math {

double* MFCC::transform(const double* source, uint32_t start, uint32_t finish, uint32_t frequency) {
	uint32_t size = finish - start + 1;

	// Calc
	double* fourierRaw = fourierTransform(source, start, finish, true);
	double** melFilters = getMelFilters(MFCC_SIZE, size, frequency, MFCC_FREQ_MIN, MFCC_FREQ_MAX);
	double* logPower = calcPower(fourierRaw, size, melFilters, MFCC_SIZE);
	double* dstTransformData = dstTransform(logPower, MFCC_SIZE);

	// Clean up
	delete [] logPower;
	delete [] fourierRaw;

	for (unsigned short m = 0; m < MFCC_SIZE; m++) {
		delete [] melFilters[m];
	}
	delete [] melFilters;

	return dstTransformData;
}

/**
 * Preemphasis digital filtration
 */
double* MFCC::filter(const double* source, uint32_t start, uint32_t finish) {
	UNUSED(source);
	UNUSED(start);
	UNUSED(finish);
	return NULL;
}

/**
 * Compute singnal's spectrum and its magnitudes (short-time Fourier transform with Hamming window)
 */
double* MFCC::fourierTransform(const double* source, uint32_t start, uint32_t finish,
		bool useWindow) {

	uint32_t size = finish - start + 1;
	complex<double>* fourierCmplxRaw = new complex<double>[size];
	double* fourierRaw = new double[size];


	for (uint32_t k = 0; k < size; k++) {
		fourierCmplxRaw[k] = complex<double>(0, 0);

		for (uint32_t n = 0; n < size; n++) {
			double sample = source[start + n];

			// According Euler's formula: e^(ix) = cos(x) + i*sin(x)
			double x = -2. * PI * k * n / (double) size;
			complex<double> f = sample * complex<double>(cos(x), sin(x));

			double w = 1.;
			if (useWindow) {
				// Hamming window
				w = 0.54 - 0.46 * cos(2 * PI * n / (size - 1));
			}

			fourierCmplxRaw[k] += f * w;
		}

		// As for magnitude, let's use Euclid's distance for its calculation
		fourierRaw[k] = sqrt(norm(fourierCmplxRaw[k]));
	}

	delete [] fourierCmplxRaw;

	return fourierRaw;
}

/**
 * Create triangular filters spaced on mel scale
 */
double** MFCC::getMelFilters(uint8_t mfccSize, uint32_t filterLength, uint32_t frequency,
		uint32_t freqMin, uint32_t freqMax) {

	// Create points for filter banks
	double* fb = new double[mfccSize + 2];
	fb[0] = convertToMel(freqMin);
	fb[mfccSize + 1] = convertToMel(freqMax);

	// Create mel bin
	for (unsigned short m = 1; m < mfccSize + 1; m++) {
		fb[m] = fb[0] + m * (fb[mfccSize + 1] - fb[0]) / (MFCC_SIZE + 1);
	}

	for (unsigned short m = 0; m < mfccSize + 2; m++) {

		// Convert them from mel to frequency
		fb[m] = convertFromMel(fb[m]);

		// Round those frequencies to the nearest DFT bin
		fb[m] = floor((filterLength + 1) * fb[m] / frequency);
	}

	// Calc filter banks
	double** filterBanks = new double*[mfccSize];
	for (unsigned short m = 0; m < mfccSize; m++) {
		filterBanks[m] =  new double[filterLength];
	}

	for (unsigned short m = 1; m < mfccSize + 1; m++) {
		for (uint32_t k = 0; k < filterLength; k++) {

			if (fb[m - 1] <= k && k <= fb[m]) {
				filterBanks[m - 1][k] = (k - fb[m - 1]) / (fb[m] - fb[m - 1]);

			} else if (fb[m] < k && k <= fb[m + 1]) {
				filterBanks[m - 1][k] = (fb[m + 1] - k) / (fb[m + 1] - fb[m]);

			} else {
				filterBanks[m - 1][k] = 0;
			}
		}
	}

	delete [] fb;

	return filterBanks;
}

/**
 * Apply mel filters to spectrum's magnitudes, take the logs of the powers
 */
double* MFCC::calcPower(const double* fourierRaw, uint32_t fourierLength,
		double** melFilters, uint32_t mfccCount) {

	double* logPower = new double[mfccCount];

	for (unsigned short m = 0; m < mfccCount; m++) {
		logPower[m] = 0.;

		for (uint32_t k = 0; k < fourierLength; k++) {
			logPower[m] += fourierRaw[k] * fourierRaw[k] * melFilters[m][k];
		}
		logPower[m] = log(logPower[m]);
	}

	return logPower;
}

/**
 * Take the discrete cosine transform of the list of mel log powers
 */
double* MFCC::dstTransform(const double* data, uint32_t length) {

	double* dstTransform = new double[length];

	for (unsigned short n = 0; n < length; n++) {
		dstTransform[n] = 0;

		for (unsigned short m = 0; m < length; m++) {
			dstTransform[n] += data[m] * cos(PI * n * (m + 1./2.) / MFCC_SIZE);
		}
	}

	return dstTransform;
}

} /* namespace math */
} /* namespace yazz */
