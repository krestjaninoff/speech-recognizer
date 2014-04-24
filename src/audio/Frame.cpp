#include <cmath>
#include <Frame.h>

namespace wtm {
namespace audio {

	Frame::Frame(length_t number, const std::vector<raw_t>& source, length_t start, length_t finish):
		number(number), source(source), start(start), finish(finish), maRms(0) {
	}

	double Frame::calcRms() const {
		double value = 0;

		for (length_t i = start; i < finish; i++) {
			value += source.at(i) * source.at(i);
		}
		value /= (finish - start);

		return sqrt(value);
	}

} /* namespace audio */
} /* namespace wtm */
