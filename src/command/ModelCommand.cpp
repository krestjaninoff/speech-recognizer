#include "ModelCommand.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <string.h>
#include <algorithm>
#include "../audio/Word.h"
#include "../audio/Processor.h"
#include "../model/Recognizer.h"

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

				cout << (*model).second->getText() << endl;
			}

		} else {
			cout << "There are no any models in the storage :( Use -a option add one!" << endl;
		}

		cout << endl;
	}

	void ModelCommand::recognize(Context& context, const string& modelNamesStr) {
		if (!context.storage->init()) {
			return;
		}
		cout << "Word recognition... ";

		// Get a word to recognize
		Word* word = getWord(context);

		// Get available models
		vector<Model*>* modelsFiltered = new vector<Model*>();
		const map<uint32_t, Model*>* models = context.storage->getModels();

		vector<string> modelNames = parseString(modelNamesStr, ',');
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

		cout << "done!" << endl;

		// Print results
		if (NULL != model) {
			cout << endl << "!!!" << endl;
			cout << "Answer is: " << model->getText();
			cout << endl << "!!!" << endl;
		} else {
			cout << "No suitable model found :(" << endl;
		}
	}

	void ModelCommand::add(Context& context, const string& modelName) {
		if (!context.storage->init()) {
			return;
		}
		cout << "Adding the new sample... ";

		// Get a word to recognize
		Word* word = getWord(context);

		// Find or create the model
		Model* theModel = NULL;
		const map<uint32_t, Model*>* models = context.storage->getModels();
		for (map<uint32_t, Model*>::const_iterator model = models->begin();
				model != models->end(); ++model) {

			if (strcmp(modelName.c_str(), (*model).second->getText().c_str())) {
				theModel = (*model).second;
				break;
			}
		}

		// Create the model if it does not exist
		string modelNameStr(modelName);
		if (NULL != theModel) {
			theModel = new Model(modelNameStr);
			context.storage->addModel(theModel);
		}

		// Add the sample to the model
		context.storage->addSample(theModel->getId(), *word);

		cout << "done!" << endl;
	}

	Word* ModelCommand::getWord(Context& context) {
		cout << "Calculating MFCC for input data... ";

		// Check pre-requirements
		if (NULL == context.wavData) {
			cerr << endl << "Input data is not specified :(" << endl;
			return NULL;
		}

		// Create the Processor
		audio::Processor* processor = new Processor(context.wavData);
		context.processor = processor;

		// Split wav data into words
		processor->split();
		if (processor->getWords()->size() > 1) {
			cerr << endl << "Sample file contains %d words (only one word is allowed)" << endl;
			return NULL;
		}

		// Calc & show mfcc
		Word* word = processor->getWords()->at(0);
		processor->initMfcc(*word);

		cout << "done!" << endl;
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



