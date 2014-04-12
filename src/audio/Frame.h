/**
 * Frame is a short fragment of a signal
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <stdint.h>
#include <vector>
#include "audio.h"

namespace wtm {
namespace audio {

class Frame {

public:
	Frame(const std::vector<raw_t>& source, lenght_t start, lenght_t finish);

	double calcRMS() const;

	lenght_t getStart() const { return start; }

	double getMaRms() const { return maRms; }
	void setMaRms(double maRms) { this->maRms = maRms; }

private:

	/**
	 * A reference to the source (wav file)
	 */
	const std::vector<raw_t>& source;

	/**
	 * First and last sample of this frame
	 */
	const lenght_t start, finish;

	/**
	 * Moving average RMS
	 */
	double maRms;
};

} /* namespace audio */
} /* namespace wtm */

#endif /* FRAME_H_ */
