#include <cmath>
#include <Frame.h>

namespace wtm {
namespace audio {

	Frame::Frame(length_t id):
		id(id), rms(0), maRms(0) {
	}

	void Frame::init(const std::vector<raw_t>& source, length_t start, length_t finish) {

		this->calcRms(source, start, finish);
		this->calcMFCC(source, start, finish);

		this->maRms = this->rms;
	}

	void Frame::calcRms(const std::vector<raw_t>& source, length_t start, length_t finish) {
		double value = 0;

		for (length_t i = start; i < finish; i++) {
			value += source.at(i) * source.at(i);
		}
		value /= (finish - start);

		this->rms = sqrt(value);
	}

	void Frame::calcMFCC(const std::vector<raw_t>& source, length_t start, length_t finish) {
		UNUSED(source);
		UNUSED(start);
		UNUSED(finish);
		// TODO Add MFCC calculation
	}

} /* namespace audio */
} /* namespace wtm */
