#include <cmath>
#include <stdlib.h>
#include <Frame.h>
#include "../math/Statistics.h"
#include "../math/MFCC.h"

namespace yazz {
namespace audio {

	Frame::Frame(uint32_t id):
		id(id), rms(0), entropy(0), mfcc(0) {
	}

	Frame::~Frame() {
		delete [] this->mfcc;
	}

	void Frame::init(const std::vector<raw_t>& source, uint32_t start, uint32_t finish,
			raw_t minRaw, raw_t maxRaw) {

		this->rms = Statistics::rms(source, start, finish);
		this->entropy = Statistics::entropy(source, start, finish, ENTROPY_BINS, minRaw, maxRaw);
	}

	double* Frame::initMFCC(const std::vector<raw_t>& source, uint32_t start,
			uint32_t finish, uint32_t fourierLength) {

		this->mfcc = MFCC::transform(source, start, finish, fourierLength);
		return this->mfcc;
	}

} /* namespace audio */
} /* namespace yazz */
