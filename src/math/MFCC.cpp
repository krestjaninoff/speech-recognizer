#include <cmath>
#include <complex>
#include <valarray>
#include <cstring>
#include <limits>
#include <assert.h>
#include <time.h>
#include "../config.h"
#include <MFCC.h>

using namespace std;

namespace yazz {
namespace math {

double* MFCC::transform(const double* source, uint32_t start, uint32_t finish, uint8_t mfccSize,
		uint32_t frequency, uint32_t freqMin, uint32_t freqMax) {
	uint32_t sampleLength = finish - start + 1;
	uint32_t p2length = pow(2, floor(log2(sampleLength)));

	// Calc
	//time_t fourierStart = time(0);
	double* fourierRaw = fourierTransformFast(source + start, p2length, true);
	//cout << "Fourier: " << time(0) - fourierStart << " sec for " << p2length << " (" << sampleLength << ") elements" << endl;

	double** melFilters = getMelFilters(mfccSize, p2length, frequency, freqMin, freqMax);
	double* logPower = calcPower(fourierRaw, p2length, melFilters, mfccSize);
	double* dctRaw = dctTransform(logPower, mfccSize);

	// Clean up
	delete [] logPower;
	delete [] fourierRaw;

	for (unsigned short m = 0; m < mfccSize; m++) {
		delete [] melFilters[m];
	}
	delete [] melFilters;

	return dctRaw;
}

/**
 * Preemphasis digital filtration
 */
double* MFCC::filter(const double* source, uint32_t start, uint32_t finish) {
	UNUSED(source);
	UNUSED(start);
	UNUSED(finish);

	// We are working with normalized data. Think that this step is excess.

	return NULL;
}

/**
 * Compute singnal's spectrum and its magnitudes (short-time Fourier transform with Hamming window)
 */
double* MFCC::fourierTransform(const double* source, uint32_t length, bool useWindow) {

	complex<double>* fourierCmplxRaw = new complex<double>[length];
	double* fourierRaw = new double[length];

	for (uint32_t k = 0; k < length; k++) {
		fourierCmplxRaw[k] = complex<double>(0, 0);

		for (uint32_t n = 0; n < length; n++) {
			double sample = source[n];

			// According Euler's formula: e^(ix) = cos(x) + i*sin(x)
			double x = -2. * M_PI * k * n / (double) length;
			complex<double> f = sample * complex<double>(cos(x), sin(x));

			double w = 1.;
			if (useWindow) {
				// Hamming window
				w = 0.54 - 0.46 * cos(2 * M_PI * n / (length - 1));
			}

			fourierCmplxRaw[k] += f * w;
		}

		// As for magnitude, let's use Euclid's distance for its calculation
		fourierRaw[k] = sqrt(norm(fourierCmplxRaw[k]));
	}

	delete [] fourierCmplxRaw;

	return fourierRaw;
}

double* MFCC::fourierTransformFast(const double* source, uint32_t length, bool useWindow) {

	// Extend source length to the power of 2
	uint32_t p2length = length;

	bool powerOfTwo = (length > 0) && !(length & (length - 1));
	assert("FFT input data size must have 2^n size" && powerOfTwo);
	// p2length = pow(2, ceil(log2(length)));

	// Move to complex calculations
	double* fourierRaw = new double[length];
	valarray<complex<double>> fourierRawTmp(p2length);

	for (uint32_t i = 0; i < p2length; i++) {

		// Even element is the real part of complex number
		if (i < length) {
			fourierRawTmp[i] = complex<double>(source[i], 0.);

			if (useWindow) {
				fourierRawTmp[i] *= (0.54 - 0.46 * cos(2 * M_PI * i / (length - 1)));
			}

		} else {
			fourierRawTmp[i] = complex<double>(0, 0);
		}
	}

	// Perform recursive calculations
	fourierTransformFastRecursion(fourierRawTmp);

	// As for magnitude, let's use Euclid's distance for its calculation
	for (uint32_t i = 0; i < length; i++) {
		fourierRaw[i] = sqrt(norm(fourierRawTmp[i]));
	}

	return fourierRaw;
}

void MFCC::fourierTransformFastRecursion(valarray<complex<double>>& data) {

	// Exit from recursion
	const size_t n = data.size();
	if (n <= 1) {
		return;
	}

	// Divide into Even/Odd
	valarray<complex<double>> even = data[std::slice(0, n/2, 2)];
	valarray<complex<double>> odd = data[std::slice(1, n/2, 2)];

	// Compute recursion
	fourierTransformFastRecursion(even);
	fourierTransformFastRecursion(odd);

	// Combine
	for (size_t i = 0; i < n / 2; i++) {
		complex<double> t = polar(1.0, -2 * M_PI * i / n) * odd[i];
		data[i]       = even[i] + t;
		data[i + n/2] = even[i] - t;
	}
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
		fb[m] = fb[0] + m * (fb[mfccSize + 1] - fb[0]) / (mfccSize + 1);
	}

	//frequency = 0.5 * frequency;
	for (unsigned short m = 0; m < mfccSize + 2; m++) {

		// Convert them from mel to frequency
		fb[m] = convertFromMel(fb[m]);

		// Map those frequencies to the nearest FT bin
		fb[m] = floor((filterLength + 1) * fb[m] / (double) frequency);

		assert("FT bin too small" &&
				!(m > 0 && (fb[m] - fb[m-1]) < numeric_limits<double>::epsilon()));
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
		double** melFilters, uint8_t mfccCount) {

	double* logPower = new double[mfccCount];

	for (unsigned short m = 0; m < mfccCount; m++) {
		logPower[m] = 0.;

		for (uint32_t k = 0; k < fourierLength; k++) {
			logPower[m] += melFilters[m][k] * pow(fourierRaw[k], 2);
		}

		assert("Spectrum power is less than zero" &&
				!(logPower[m] < numeric_limits<double>::epsilon()));

		// NOTE I'm not sure that we need to take logs since we normalized the input data
		logPower[m] = log(logPower[m]);
	}

	return logPower;
}

/**
 * Take the discrete cosine transform of the list of mel log powers
 */
double* MFCC::dctTransform(const double* data, uint32_t length) {

	double* dctTransform = new double[length];

	for (unsigned short n = 0; n < length; n++) {
		dctTransform[n] = 0;

		for (unsigned short m = 0; m < length; m++) {
			dctTransform[n] += data[m] * cos(M_PI * n * (m + 1./2.) / length);
		}
	}

	return dctTransform;
}

} /* namespace math */
} /* namespace yazz */
