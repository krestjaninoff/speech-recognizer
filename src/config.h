/**
 * Project's settings
 */

#ifndef COMMON_H_
#define COMMON_H_

#define __GXX_EXPERIMENTAL_CXX0X__ 1

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <limits.h>

namespace yazz {

	// Build information

	#define PROJECT_NAME "YAZZ"
	#define PROJECT_VERSION "0.0.2"

	/**
	 * Raw audio data type
	 */
	typedef int16_t raw_t;

	/**
	 * Length of frame (ms)
	 */
	const uint32_t FRAME_LENGTH = 50;

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
	 * Amount of MFCC coefficients
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

	/**
	 * Codebook threshold
	 */
	const double CODEBOOK_THRESHOLD = 25;


	// Helpers

	#define DEBUG_ENABLED false
	#define DEBUG(...)  \
		do { if (DEBUG_ENABLED) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); } } while (0)

	#define UNUSED(expr) do { (void)(expr); } while (0)

	#define EOL "\n"
	#define toString( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


	// Test settings

	const double EPS_TEST = 1e-4;

} // namespace yazz

#endif /* COMMON_H_ */
