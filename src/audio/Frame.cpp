#include <cmath>
#include <Frame.h>
#include "../math/MFCC.h"

namespace yazz {
namespace audio {

	Frame::Frame(uint32_t id):
		id(id), rms(0), maRms(0), mfcc(0) {
	}

	void Frame::init(const std::vector<raw_t>& source, uint32_t start, uint32_t finish) {

		this->calcRms(source, start, finish);
		this->maRms = this->rms;
	}

	void Frame::calcRms(const std::vector<raw_t>& source, uint32_t start, uint32_t finish) {
		double value = 0;

		for (uint32_t i = start; i < finish; i++) {
			value += source.at(i) * source.at(i);
		}
		value /= (finish - start);

		this->rms = sqrt(value);
	}

	double* Frame::initMFCC(const std::vector<raw_t>& source, uint32_t start,
			uint32_t finish, uint32_t fourierLength) {

		this->mfcc = MFCC::transform(source, start, finish, fourierLength);
		return this->mfcc;
	}

	Frame::~Frame() {
		delete [] this->mfcc;
	}

} /* namespace audio */
} /* namespace yazz */
