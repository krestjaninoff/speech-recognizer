#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <Processor.h>
#include <stddef.h>
#include <Storage.h>
#include <WavData.h>

using namespace yazz::audio;
using namespace yazz::model;

namespace yazz {
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
} /* namespace yazz */

#endif /* CONTEXT_H_ */
