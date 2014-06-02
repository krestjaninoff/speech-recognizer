#include <MFCC.h>
#include <stddef.h>

#include "../common.h"

namespace yazz {
namespace math {

double* MFCC::transform(const double* source, uint32_t start, uint32_t finish, uint32_t frequency) {
	uint32_t size = finish - start + 1;

	// Calc
	double* fourierRaw = fourierTransform(source, start, finish, true);
	double** melFilters = getMelFilters(size, frequency);
	double* logPower = calcPower(fourierRaw, melFilters, size);
	double* dstTransformData = dstTransform(logPower);

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
 * Perform short-time fourier transform with Hamming windows
 */
double* MFCC::fourierTransform(const double* source, uint32_t start, uint32_t finish, bool useWindow) {

	uint32_t size = finish - start + 1;
	double* fourierRaw = new double[size];

	for (uint32_t k = 0; k < size; k++) {
		fourierRaw[k] = 0;

		for (uint32_t n = 0; n < size; n++) {
			double sample = source[start + n];

			// e^(ix) = cos(x) + i*sin(x)
			double x = -2. * PI * k * n / size;
			double f = sample * (cos(x) + sin(x));

			double w = 1;
			if (useWindow) {
				// Hamming window
				w = 0.54 + 0.46 * cos(2 * PI * n / (size - 1));
			}
			fourierRaw[k] += f * w;
		}

		fourierRaw[k] = fabs(fourierRaw[k]);
	}

	return fourierRaw;
}

/**
 * Create mel filters, using triangular overlapping windows
 */
double** MFCC::getMelFilters(uint32_t fourierLength, uint32_t frequency) {

	// Create points for filter banks
	double* fb = new double[MFCC_SIZE + 2];
	fb[0] = convertToMel(MFCC_FREQ_MIN);
	fb[MFCC_SIZE + 1] = convertToMel(MFCC_FREQ_MAX);

	// Create mel bin
	for (unsigned short m = 1; m < MFCC_SIZE + 1; m++) {
		fb[m] = fb[0] + m * (fb[MFCC_SIZE + 1] - fb[0]) / (MFCC_SIZE + 1);
	}

	for (unsigned short m = 0; m < MFCC_SIZE + 2; m++) {

		// Convert them from mel to frequency
		fb[m] = convertFromMel(fb[m]);

		// Round those frequencies to the nearest DFT bin
		fb[m] = floor((fourierLength + 1) * fb[m] / frequency);
	}

	// Calc filter banks
	double** filterBanks = new double*[MFCC_SIZE];
	for (unsigned short m = 0; m < MFCC_SIZE; m++) {
		filterBanks[m] =  new double[fourierLength];
	}

	for (unsigned short m = 1; m < MFCC_SIZE + 1; m++) {
		for (uint32_t k = 0; k < fourierLength; k++) {

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
 * Take the logs of the powers at each of the mel frequencies
 */
double* MFCC::calcPower(const double* fourierRaw, double** melFilters, uint32_t fourierLength) {

	double* logPower = new double[MFCC_SIZE];

	for (unsigned short m = 0; m < MFCC_SIZE; m++) {
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
double* MFCC::dstTransform(const double* logPower) {

	double* dstTransform = new double[MFCC_SIZE];

	for (unsigned short n = 0; n < MFCC_SIZE; n++) {
		dstTransform[n] = 0;

		for (unsigned short m = 0; m < MFCC_SIZE; m++) {
			dstTransform[n] += logPower[m] * cos(PI * n * (m + 1./2.) / MFCC_SIZE);
		}
	}

	return dstTransform;
}

} /* namespace math */
} /* namespace yazz */
