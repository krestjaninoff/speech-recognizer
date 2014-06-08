#include <Model.h>
#include <ModelCommand.h>
#include <Processor.h>
#include <Recognizer.h>
#include <string.h>
#include <Storage.h>
#include <WavData.h>
#include <Word.h>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace yazz {
namespace command {

	void ModelCommand::list(Context& context) {
		if (!context.storage->init()) {
			return;
		}

		const map<uint32_t, Model*>* models = context.storage->getModels();

		if (models->size() > 0) {
			cout << "Available models are:" << endl;

			for (map<uint32_t, Model*>::const_iterator model = models->begin();
					model != models->end(); ++model) {

				cout << " - \"" <<  (*model).second->getText() << "\" (" <<
						(*model).second->getSamples().size() << ")" << endl;
			}

		} else {
			cout << "There are no any models in the storage :( Use -a option add one!" << endl;
		}

		cout << endl;
	}

	void ModelCommand::add(Context& context, const char* modelNameChar) {

		// Check the model's name
		if (NULL == modelNameChar) {
			cout << "Model name is not specified" << endl;
			return;
		}
		string modelName(modelNameChar);

		cout << "Adding the new sample... " << endl;

		// Get a word to recognize
		Word* word = getWord(context);
		if (NULL == word) {
			return;
		}

		// Inin storage
		if (!context.storage->init()) {
			return;
		}

		// Find or create the model
		Model* theModel = NULL;
		const map<uint32_t, Model*>* models = context.storage->getModels();
		for (map<uint32_t, Model*>::const_iterator model = models->begin();
				model != models->end(); ++model) {

			if (0 == strcmp(modelName.c_str(), (*model).second->getText().c_str())) {
				theModel = (*model).second;
				break;
			}
		}

		// Create the model if it does not exist
		string modelNameStr(modelName);
		if (NULL == theModel) {
			theModel = new Model(modelNameStr);
			context.storage->addModel(theModel);
		}

		// Add the sample to the model
		context.storage->addSample(theModel->getId(), *word);
		context.storage->persist();

		cout << "The new sample has been successfully added!" << endl;
	}

	string ModelCommand::doRecognize(Context& context, const char* modelNamesChar) {

		// Check the storage
		if (!context.storage->init()) {
			return NULL;
		}
		if (0 == context.storage->getModels()->size()) {
			cout << "Models storage is empty! Add some model before starting recognition." << endl;
			return NULL;
		}

		cout << "Word recognition... " << endl;

		// Get a word to recognize
		Word* word = getWord(context);

		// Get available models
		vector<Model*>* modelsFiltered = new vector<Model*>();
		const map<uint32_t, Model*>* models = context.storage->getModels();

		vector<string> modelNames;
		if (NULL != modelNamesChar) {
			modelNames = parseString(modelNamesChar, ',');
		}

		for (map<uint32_t, Model*>::const_iterator model = models->begin();
				model != models->end(); ++model) {

			string modelName((*model).second->getText());
			if (0 == modelNames.size() ||
					find(modelNames.begin(), modelNames.end(), modelName) != modelNames.end()) {

				modelsFiltered->push_back((*model).second);
			}
		}

		// Try to recognize
		Recognizer rec(modelsFiltered);
		const Model* model = rec.recognize(*word);
		string theWord = NULL;

		// Print results
		if (NULL != model) {
			theWord =  model->getText();
		}

		return theWord;
	}

	void ModelCommand::recognize(Context& context, const char* modelNamesChar) {
		string theWord = doRecognize(context, modelNamesChar);

		// Print results
		if (theWord.empty()) {
			cout << endl << "!!!" << endl;
			cout << "The answer is: " << theWord;
			cout << endl << "!!!" << endl;

		} else {
			cout << "No suitable model was found :(" << endl;
		}
	}

	Word* ModelCommand::getWord(Context& context) {

		// Check pre-requirements
		if (NULL == context.wavData) {
			cerr << "Input data is not specified :(" << endl;
			return NULL;
		}

		cout << "Checking input data... " << endl;

		// Create the Processor
		Processor* processor = new Processor(context.wavData);
		context.processor = processor;

		// Split wav data into words
		processor->split();
		if (processor->getWords()->size() != 1) {
			cerr << "Sample file contains " << processor->getWords()->size()
					<< " words, but we need exactly ONE word" << endl;
			return NULL;
		}

		cout << "Calculating MFCC for input data... " << endl;

		// Calc & show mfcc
		Word* word = processor->getWords()->at(0);
		processor->initMfcc(*word);

		return word;
	}

	vector<string> ModelCommand::parseString(const string& source, char delimeter) {
		stringstream ss(source);
		vector<string> result;

		while(ss.good()) {
		   string substr;

		   getline(ss, substr, delimeter);
		   result.push_back(substr);
		}

		return result;
	}

} /* namespace command */
} /* namespace yazz */



