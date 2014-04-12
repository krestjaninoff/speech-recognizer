#include <cmath>
#include <Frame.h>

namespace wtm {
namespace audio {

	Frame::Frame(const std::vector<raw_t>& source, lenght_t start, lenght_t finish):
		source(source), start(start), finish(finish), maRms(0) {
	}

	double Frame::calcRMS() const {
		double value = 0;

		for (lenght_t i = start; i < finish; i++) {
			value += source.at(i) * source.at(i);
		}
		value /= (finish - start);

		return sqrt(value);
	}

} /* namespace audio */
} /* namespace wtm */
