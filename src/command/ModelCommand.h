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
	 * Display the list of available models (print CodeBook)
	 */
	static void listLabels(Context& context);

	/**
	 * Add a new label (or update existing one)
	 */
	static void addLabel(Context& context, const char* label);

	/**
	 * Remove the label
	 */
	static void removeLabel(Context& context, const char* label);


	/**
	 * Display the list of available models
	 */
	static void listModels(Context& context);

	/**
	 * Display the model by its is
	 */
	static void displayModel(Context& context, const char* modelId);

	/**
	 * Create a new model
	 */
	static void addModel(Context& context, const char* modelName);

	/**
	 * Train the model
	 */
	static void trainModel(Context& context, const char* modelId);

	/**
	 * Delete the model
	 */
	static void deleteModel(Context& context, const char* modelId);


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
