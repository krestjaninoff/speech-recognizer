#include <complex.h>
#include <cmath>
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

	// Calc
	time_t fourierStart = time(0);
	double* fourierRaw = fourierTransform(source + start, sampleLength, true);
	cout << "Fourier: " << time(0) - fourierStart << "sec for " << sampleLength << " elements" << endl;

	double** melFilters = getMelFilters(mfccSize, sampleLength, frequency, freqMin, freqMax);
	double* logPower = calcPower(fourierRaw, sampleLength, melFilters, mfccSize);
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
	if (!powerOfTwo) {
		p2length = 2 * ceil(log2(length));
	}

	// Prepare data (real + imagine)
	uint32_t n = p2length * 2;

	double* fourierRaw = new double[p2length];
	double* fourierRawTmp = new double[n];

	int index;
	for (uint32_t i = 0; i < n; i += 2) {
		fourierRawTmp[i] = 0;
		index = i / 2;

		if (index < length) {
			fourierRawTmp[i + 1] = source[index];

			if (useWindow) {
				fourierRawTmp[i + 1] *= (0.54 - 0.46 * cos(2 * M_PI * index / (length - 1)));
			}
		} else {
			fourierRawTmp[i + 1] = 0;
		}
	}

	// Reverse-binary reindexing
	uint32_t i = 1;
	uint32_t j = 1;
	uint32_t m;
	double tmpReal, tmpImg;
	while (i < n) {
		if (j > i) {
			tmpReal = fourierRawTmp[i];
			fourierRawTmp[i] = fourierRawTmp[j];
			fourierRawTmp[j] = tmpReal;

			tmpReal = fourierRawTmp[i + 1];
			fourierRawTmp[i + 1] = fourierRawTmp[j + 1];
			fourierRawTmp[j + 1] = tmpReal;
		}

		i = i + 2;
		m = p2length;
		while ((m >= 2) && (j > m)) {
			j = j - m;
			m = m >> 1;
		}
		j = j + m;
	}

	// Danielson-Lanczos section
	uint32_t mMax = 2;
	uint32_t mMax2;
	double sinTheta2Sq, sinTheta, wReal, wImagine, sinTheta2, theta;

	while (n > mMax) {
		theta = -2 * M_PI / mMax;
		sinTheta = sin(theta);
		sinTheta2 = sin(theta / 2);
		sinTheta2Sq = sinTheta2 * sinTheta2 * 2;
		mMax2 = mMax * 2;
		wReal = 1;
		wImagine = 0;
		m = 1;

		while (m < mMax) {
			i = m;
			m = m + 2;
			tmpReal = wReal;
			tmpImg = wImagine;
			wReal = wReal - tmpReal * sinTheta2Sq - tmpImg * sinTheta;
			wImagine = wImagine + tmpReal * sinTheta - tmpImg * sinTheta2Sq;

			while (i < n) {
				j = i + mMax;
				tmpReal = wReal * fourierRawTmp[j] - wImagine * fourierRawTmp[j - 1];
				tmpImg = wImagine * fourierRawTmp[j] + wReal * fourierRawTmp[j - 1];

				fourierRawTmp[j] = fourierRawTmp[i] - tmpReal;
				fourierRawTmp[j - 1] = fourierRawTmp[i - 1] - tmpImg;
				fourierRawTmp[i] = fourierRawTmp[i] + tmpReal;
				fourierRawTmp[i - 1] = fourierRawTmp[i - 1] + tmpImg;
				i = i + mMax2;
			}
		}

		mMax = mMax2;
	}

	// Calculate magnitude
	double val;
	int position;
	for (i = 0; i < p2length; i++) {
		j = i * 2;
		position = p2length - i - 1;

		fourierRaw[position] = sqrt(pow(fourierRawTmp[j], 2) + pow(fourierRawTmp[j + 1], 2));
	}

	delete[] fourierRawTmp;

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
