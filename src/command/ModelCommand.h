#ifndef MODELCOMMAND_H_
#define MODELCOMMAND_H_

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "Context.h"
#include "../audio/Word.h"
#include "../audio/Processor.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace command {

/**
 * Work with models
 */
class ModelCommand {
public:

	/**
	 * Display the list of available models
	 */
	static void list(Context& context);

	/**
	 * Recognize input data using specified models (or all available models)
	 */
	static void recognize(Context& context, const string& modelNames);

	/**
	 * Add current sample into the model
	 */
	static void add(Context& context, const string& modelName);

private:

	static Word* getWord(Context& context);

	static vector<string> parseString(const string& source, char delimeter);
};

} /* namespace command */
} /* namespace wtm */

#endif /* MODELCOMMAND_H_ */
