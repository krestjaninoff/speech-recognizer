#ifndef MFCC_H_
#define MFCC_H_

#include <stdint.h>
#include <vector>
#include "matf.h"
#include "../audio/audio.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace math {

/**
 * Helper for transformation raw audio data into mel-frequency cepstrum
 *
 * @see http://en.wikipedia.org/wiki/Mel-frequency_cepstrum
 * @see http://www.pvsm.ru/programmirovanie/4344
 */
class MFCC {
public:

	/**
	 * Perform MFCC transformation
	 */
	static const double* transform(const std::vector<raw_t>& source, length_t start, length_t finish);

// The methods below should be private, but for testing purposes they are public now
// private:

	/**
	 * Perform short-time fourier transform with Hamming windows
	 *
	 * @see http://en.wikipedia.org/wiki/Fourier_transform
	 * @see http://en.wikipedia.org/wiki/Discrete-time_Fourier_transform
	 * @see http://en.wikipedia.org/wiki/Short-time_Fourier_transform
	 * @see http://en.wikipedia.org/wiki/Window_function#Generalized_Hamming_windows
	 */
	static const double* fourierTransform(const std::vector<raw_t>& source, length_t start, length_t finish);

	/**
	 * Map the powers of the spectrum obtained above onto the mel scale, using triangular overlapping windows
	 *
	 * @see http://en.wikipedia.org/wiki/Window_function#Triangular_window
	 * @see http://en.wikipedia.org/wiki/Mel_scale
	 */
	static const double* melFreq(const double* fourierRaw, uint32_t size, uint8_t mfccSize);

	/**
	 * Take the logs of the powers at each of the mel frequencies
	 */
	static const double* logPower(const double* melFreq, uint8_t mfccSize);

	/**
	 * Take the discrete cosine transform of the list of mel log powers
	 *
	 * @see http://en.wikipedia.org/wiki/Discrete_cosine_transform#DCT-II
	 */
	static const double* dstTransform(const double* melScale, uint8_t mfccSize);
};

} /* namespace math */
} /* namespace wtm */

#endif /* MFCC_H_ */
