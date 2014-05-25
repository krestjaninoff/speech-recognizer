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
	Frame(length_t id);
	~Frame();

	/**
	 * Init the frame using a part of wave data
	 */
	void init(const std::vector<raw_t>& source, length_t start, length_t finish);

	/**
	 * Get frame's serial number
	 */
	length_t getId() const { return this->id; }

	/**
	 * Get Root Mean Square
	 */
	double getRms() const { return rms; }

	/**
	 * Get Moving Average value of RMS
	 */
	double getMaRms() const { return maRms; }
	void setMaRms(double maRms) { this->maRms = maRms; }

	/**
	 * Get Mel-frequency cepstral coefficients
	 */
	double* getMFCC() const { return mfcc; }
	double* initMFCC(const std::vector<raw_t>& source, length_t start,
			length_t finish, uint32_t fourierLength);


private:
	const length_t id;

	double rms;
	double maRms;

	double* mfcc;

	void calcRms(const std::vector<raw_t>& source, length_t start, length_t finish);
};

} /* namespace audio */
} /* namespace yazz */

#endif /* FRAME_H_ */
