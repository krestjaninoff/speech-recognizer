#ifndef MODELCOMMAND_H_
#define MODELCOMMAND_H_

#include "Context.h"

using namespace std;
using namespace yazz::audio;

namespace yazz {
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
	 * Add current sample into the model
	 */
	static void add(Context& context, const char* modelName);

	/**
	 * Recognize input data using specified models (or all available models)
	 */
	static void recognize(Context& context, const char* modelNames);

	/**
	 * Recognize input data using specified models (or all available models)
	 */
	static string doRecognize(Context& context, const char* modelNames);

private:

	static Word* getWord(Context& context);

	static vector<string> parseString(const string& source, char delimeter);
};

} /* namespace command */
} /* namespace yazz */

#endif /* MODELCOMMAND_H_ */
