#ifndef MODELCOMMAND_H_
#define MODELCOMMAND_H_

#include <stdio.h>
#include <iostream>
#include <fstream>
//#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ICommand.h"
#include "../audio/Word.h"
#include "../audio/Processor.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace command {

/**
 * Work with models
 */
class ModelCommand : public ICommand {
public:

	bool execute(Context& context) {
		int counter = 1;
		cout << "Splitting data into separate words..." << endl;

		// Check pre-requirements
		if (NULL == context.wavData) {
			cout << "Input data is not specified :(" << endl;
			return false;
		}

		// Create the Processor
		audio::Processor* processor = new Processor(context.wavData);
		context.processor = processor;

		// Split wav data into words
		processor->split();
		if (processor->getWords()->size() > 1) {
			cout << "Training sample contains %d words (only one word is allowed)" << endl;
			return false;
		}

		// Calc & show mfcc
		Word* word = processor->getWords()->at(0);
		processor->initMfcc(*word);

		cout << "Complete!" << endl;
		return true;
	};
};

} /* namespace command */
} /* namespace wtm */

#endif /* MODELCOMMAND_H_ */
