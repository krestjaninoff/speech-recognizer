#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "../audio/WavData.h"
#include "../audio/Processor.h"
#include "Storage.h"

using namespace wtm::audio;

namespace wtm {
namespace command {

/**
 * Command execution context
 */
class Context {
public:

	Context() {
		this->wavData = NULL;
		this->processor = NULL;
		this->storage = new Storage();
	}

	~Context() {
		delete this->storage;

		if (NULL != this->processor) {
			delete this->processor;
		}

		if (NULL != this->wavData) {
			delete this->wavData;
		}
	}

	WavData* wavData;
	Processor* processor;
	Storage* storage;
};

} /* namespace command */
} /* namespace wtm */

#endif /* CONTEXT_H_ */
