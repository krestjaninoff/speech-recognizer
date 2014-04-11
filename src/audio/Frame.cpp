#include <Frame.h>
#include <cmath>

namespace wtm {
namespace audio {

	Frame::Frame(const std::vector<raw_t>& source, lenght_t start, lenght_t finish):
		source(source), start(start), finish(finish) {
	}

	double Frame::getAvgValue() const {
		double avgValue = source.at(start);

		for (int i = start + 1; i < finish; i++) {
			avgValue = (avgValue + source.at(i)) / 2;
		}

		return abs(avgValue);
	}

} /* namespace audio */
} /* namespace wtm */
