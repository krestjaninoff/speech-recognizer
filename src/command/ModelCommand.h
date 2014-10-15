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
	 * Display the list of available observations (print CodeBook)
	 */
	static void listCodebook(Context& context);

	/**
	 * Display the MFCC data by for the observation
	 */
	static void printCodebookEntry(Context& context, const char* observation);

	/**
	 * Add a new observation (or update existing one)
	 */
	static void addCodebookEntry(Context& context, const char* observation);

	/**
	 * Remove the observation
	 */
	static void deleteCodebookEntry(Context& context, const char* observation);


	/**
	 * Display the list of available models
	 */
	static void listModels(Context& context);

	/**
	 * Display the model by its id
	 */
	static void printModel(Context& context, const char* modelIdStr);

	/**
	 * Create a new model
	 */
	static void addModel(Context& context, const char* modelName);


	/**
	 * Display observations retrieved from the input data
	 */
	static void displayObservations(Context& context);

	/**
	 * Train the model
	 */
	static void trainModel(Context& context, const char* modelIdStr);

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

	static const vector<MfccEntry*>* getMfccData(Context& context);

	static vector<string> parseString(const string& source, char delimeter);
};

} /* namespace command */
} /* namespace yazz */

#endif /* MODELCOMMAND_H_ */
