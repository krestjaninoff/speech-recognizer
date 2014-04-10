#ifndef FRAME_H_
#define FRAME_H_

#include <stdint.h>
#include <vector>

namespace wtm {
namespace audio {

/**
 * Frame is a short fragment of a signal
 */
class Frame {

public:
	Frame(const std::vector<int>& source, int32_t start, int32_t finish);

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
