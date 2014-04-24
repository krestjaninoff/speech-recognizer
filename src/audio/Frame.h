#ifndef FRAME_H_
#define FRAME_H_

#include <stdint.h>
#include <vector>
#include "audio.h"

namespace wtm {
namespace audio {

/**
 * Frame is a short fragment of a signal
 */
class Frame {

public:

	/**
	 * Create a frame based on a part of wave data
	 */
	Frame(length_t number, const std::vector<raw_t>& source, length_t start, length_t finish);

	/**
	 * Calculate Root Mean Square for the frame
	 */
	double calcRms() const;

	/**
	 * Get Moving Average value of RMS
	 */
	double getMaRms() const { return maRms; }
	void setMaRms(double maRms) { this->maRms = maRms; }

	/**
	 * Get frame's serial number
	 */
	length_t getNumber() const { return this->number; }

	length_t getStart() const { return this->start; }
	length_t getFinish() const { return this->finish; }

private:

	/**
	 * Reference to the source (wav data)
	 */
	const std::vector<raw_t>& source;

	/**
	 * First and last sample of this frame
	 */
	const length_t start, finish;

	/**
	 * Index (serial) number
	 */
	const length_t number;

	/**
	 * Moving average RMS
	 */
	double maRms;
};

} /* namespace audio */
} /* namespace wtm */

#endif /* FRAME_H_ */
