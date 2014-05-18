#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "../audio/WavData.h"
#include "../audio/Processor.h"

using namespace wtm::audio;

namespace wtm {
namespace command {

/**
 * Command execution context
 */
class Context {
public:
	WavData* wavData;
	Processor* processor;
};

} /* namespace command */
} /* namespace wtm */

#endif /* CONTEXT_H_ */
