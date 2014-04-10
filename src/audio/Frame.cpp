#include <Frame.h>

namespace wtm {
namespace audio {

	Frame::Frame(const std::vector<int>& source, int32_t start, int32_t finish):
		start(start), finish(finish), source(source) {
	}

} /* namespace audio */
} /* namespace wtm */
