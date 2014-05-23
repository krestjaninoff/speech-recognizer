#ifndef MFCC_H_
#define MFCC_H_

#include <stdint.h>
#include <vector>
#include <math.h>
#include "maph.h"
#include "../audio/audio.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace math {

/**
 * Helper for transformation raw audio data into mel-frequency "cepstrum"
 *
 * @see http://en.wikipedia.org/wiki/Mel-frequency_cepstrum
 * @see http://www.pvsm.ru/programmirovanie/4344
 */
class MFCC {
public:

	/**
	 * Perform MFCC transformation
	 */
	static double* transform(const std::vector<raw_t>& source, length_t start, length_t finish, uint32_t frequency);

// The methods below should be private, but for testing purposes they are public now
// private:

	/**
	 * Perform short-time fourier transform with Hamming windows
	 *
	 * @see http://en.wikipedia.org/wiki/Fourier_transform
	 * @see http://en.wikipedia.org/wiki/Discrete-time_Fourier_transform
	 * @see http://en.wikipedia.org/wiki/Short-time_Fourier_transform
	 * @see http://en.wikipedia.org/wiki/Window_function#Generalized_Hamming_windows
	 *
	 * @see http://www.robots.ox.ac.uk/~sjrob/Teaching/SP/l6.pdf
	 * @see http://www.robots.ox.ac.uk/~sjrob/Teaching/SP/l7.pdf
	 */
	static double* fourierTransform(const std::vector<raw_t>& source, length_t start, length_t finish, bool useWindow);

	/**
	 * Create mel filters (for range of frequencies), using triangular overlapping windows
	 *
	 * @see http://en.wikipedia.org/wiki/Mel_scale
	 * @see http://en.wikipedia.org/wiki/Window_function#Triangular_window
	 * @see http://practicalcryptography.com/miscellaneous/machine-learning/guide-mel-frequency-cepstral-coefficients-mfccs/
	 * @see http://www.pvsm.ru/programmirovanie/4344
	 */
	static double** getMelFilters(uint32_t size, uint32_t frequency);

	/**
	 * Take the logs of the powers at each of the mel frequencies
	 */
	static double* calcPower(double* fourierRaw, double** melFilters, uint32_t fourierLength);

	/**
	 * Take the discrete cosine transform of the list of mel log powers
	 *
	 * @see http://en.wikipedia.org/wiki/Discrete_cosine_transform#DCT-II
	 */
	static double* dstTransform(double* melScale);


	// Mel convertors
	static double convertToMel(double f) { return 1125. * log(1. + f/700.); }
	static double convertFromMel(double m) { return 700. * (exp(m/1125.) - 1); }
};

} /* namespace math */
} /* namespace wtm */

#endif /* MFCC_H_ */
