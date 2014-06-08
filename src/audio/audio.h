/**
 * Common objects for work with audio
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <string>
#include <list>
#include <memory>
#include <cstdlib>
#include <stdint.h>
#include "../common.h"

namespace yazz {
namespace audio {

	/**
	 * Raw audio data type
	 */
	typedef int16_t raw_t;

	/**
	 * Length of frame (ms)
	 */
	const uint32_t FRAME_LENGTH = 10;

	/**
	 * Percentage of overlap for frames (0 <= x < 1)
	 */
	const double FRAME_OVERLAP = 0.5;

	/**
	 * Minimal size of word (in frames)
	 * <p>
	 * According my experiments average length of the words in my dictionary is 500ms.
	 * Let's put that the minimal length of word is 200ms.
	 */
	const unsigned short WORD_MIN_SIZE = (200 / FRAME_LENGTH) / (1 - FRAME_OVERLAP);

	/**
	 * Minimal amount of framer between two words
	 * <p>
	 * Let's put that minimal distance between two words is 50% of minimal size of word
	 */
	const unsigned short WORDS_MIN_DISTANCE = WORD_MIN_SIZE * 0.50;

	/**
	 * MFCC vector size
	 */
	const unsigned short MFCC_SIZE = 12;

	/**
	 * Frequency bounds
	 */
	const short MFCC_FREQ_MIN = 300;
	const short MFCC_FREQ_MAX = 4000;

	/**
	 * Entropy parameters
	 */
	const short ENTROPY_BINS = 75;
	const double ENTROPY_THRESHOLD = 0.1;

} // namespace audio
} // namespace yazz

#endif /* AUDIO_H_ */
