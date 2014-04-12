/*
 * Common objects for work with audio
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <string>
#include <list>
#include <memory>
#include <cstdlib>
#include <stdint.h>

namespace wtm {
namespace audio {

	/**
	 * Raw audio data type
	 */
	typedef int32_t raw_t;

	/**
	 * Raw audio data type
	 */
	typedef uint32_t lenght_t;


	/**
	 * Length of frame (ms)
	 */
	const lenght_t FRAME_LENGTH = 25;

	/**
	 * Percentage of overlap for frames (0 <= x < 1)
	 */
	const double FRAME_OVERLAP = 0.5;

	/**
	 * Amount (odd) of elements for moving average smoothing during words splitting
	 * Length of analyzing fragment is FRAME_LENGTH * (1 - FRAME_OVERLAP) * MA_SIZE
	 */
	const unsigned short MOVING_AVERAGE_SIZE = 5;

} // namespace audio
} // namespace wtm

#endif /* AUDIO_H_ */
