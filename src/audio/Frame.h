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
	Frame(const std::vector<raw_t>& source, lenght_t start, lenght_t finish);

	double getAvgValue() const;

private:
	/**
	 * First and last sample of this frame
	 */
	const uint32_t start, finish;

	/**
	 * A reference to the source (wav file)
	 */
	const std::vector<int32_t>& source;
};

} /* namespace audio */
} /* namespace wtm */

#endif /* FRAME_H_ */
