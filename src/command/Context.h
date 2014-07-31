#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "../audio/Processor.h"
#include "../model/Processor.h"
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
		this->audioProcessor = NULL;
		this->modelProcessor = NULL;
		this->storage = NULL;
	}

	~Context() {
		delete this->storage;

		if (NULL != this->audioProcessor) {
			delete this->audioProcessor;
		}

		if (NULL != this->wavData) {
			delete this->wavData;
		}
	}

	WavData* getWavData() { return this->wavData; }
	void setWavData(WavData* wavData) { this->wavData = wavData; }

	audio::Processor* getAudioProcessor() { return this->audioProcessor; }
	void setAudioProcessor(audio::Processor* audioProcessor) { this->audioProcessor = audioProcessor; }

	model::Processor* getModelProcessor() {
		if (NULL == this->modelProcessor) {
			this->modelProcessor = new model::Processor(this->getStorage());
			this->storage->init();
		}
		return this->modelProcessor;
	}
	//void setModelProcessor(model::Processor* modelProcessor) { this->modelProcessor = modelProcessor; }

	Storage* getStorage() {
		if (NULL == this->storage) {
			this->storage = new Storage();
			this->storage->init();
		}
		return this->storage;
	}
	//void setStorage() { this->storage = storage; }

private:
	WavData* wavData;
	audio::Processor* audioProcessor;
	model::Processor* modelProcessor;
	Storage* storage;
};

} /* namespace command */
} /* namespace yazz */

#endif /* CONTEXT_H_ */
