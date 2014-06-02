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

	void Frame::init(const double* source, uint32_t start, uint32_t finish) {

		this->rms = Statistics::rms(source, start, finish);
		this->entropy = Statistics::entropy(source, start, finish, ENTROPY_BINS, -1, 1);
	}

	double* Frame::initMFCC(const double* source, uint32_t start, uint32_t finish, uint32_t frenq) {

		this->mfcc = MFCC::transform(source, start, finish, frenq);
		return this->mfcc;
	}

} /* namespace audio */
} /* namespace yazz */
