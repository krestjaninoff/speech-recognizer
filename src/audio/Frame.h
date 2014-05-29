#ifndef FRAME_H_
#define FRAME_H_

#include <stdint.h>
#include <vector>
#include "audio.h"
#include "../math/MFCC.h"

using namespace yazz::math;

namespace yazz {
namespace audio {

/**
 * Frame is a short fragment of a signal
 */
class Frame {

public:
	/**
	 * Create a frame
	 */
	Frame(uint32_t id);
	~Frame();

	/**
	 * Init the frame using a part of wave data
	 */
	void init(const std::vector<raw_t>& source, uint32_t start, uint32_t finish,
			raw_t minRaw, raw_t maxRaw);

	/**
	 * Get frame's serial number
	 */
	uint32_t getId() const { return this->id; }

	/**
	 * Get Root Mean Square
	 */
	double getRms() const { return rms; }
	double getEntropy() const { return entropy; }

	/**
	 * Get Mel-frequency cepstral coefficients
	 */
	double* getMFCC() const { return mfcc; }
	double* initMFCC(const std::vector<raw_t>& source, uint32_t start,
			uint32_t finish, uint32_t fourierLength);

private:
	const uint32_t id;

	double rms;
	double entropy;

	double* mfcc;
};

} /* namespace audio */
} /* namespace yazz */

#endif /* FRAME_H_ */
