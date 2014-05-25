#include "ModelCommand.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <string.h>

namespace yazz {
namespace command {

	void ModelCommand::list(Context& context) {
		context.storage->init();
		map<uint32_t, Model*>* models = context.storage->getModels();

		cout << "Available models are:" << endl;

		for (vector<Model*>::const_iterator model = models->begin();
				model != models->end(); ++model) {

			cout << (*model)->getText() << endl;
		}

		cout << endl;
	}

	void ModelCommand::recognize(Context& context, const string& modelNamesStr) {
		cout << "Word recognition... ";

		// Get a word to recognize
		Word* word = getWord(context);

		// Get available models
		vector<Model*> modelsFiltered = new vector<Model*>();
		map<uint32_t, Model*>* models = context.storage->getModels();

		vector<string> modelNames = parseString(modelNamesStr, ',');
		for (vector<Model*>::const_iterator model = models->begin();
			model != models->end(); ++model) {

			// TODO Implement filtration
			modelsFiltered.push_back(*model);
		}

		// Try to recognize
		Recognizer rec(modelsFiltered);
		Model* model = rec.recognize(*word);

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
		cout << "Adding the new sample... ";

		// Get a word to recognize
		Word* word = getWord(context);

		// Find or create the model
		Model* theModel = NULL;
		map<uint32_t, Model*>* models = context.storage->getModels();
		for (vector<Model*>::const_iterator model = models->begin();
			model != models->end(); ++model) {

			if (strcmp(modelName.c_str(), (*model)->getText().c_str())) {
				theModel = *model;
				break;
			}
		}

		// Create the model if it does not exist
		if (NULL != theModel) {
			theModel = new Model(modelName);
			context.storage->addModel(theModel);
		}

		// Add the sample to the model
		context.storage->addSample(theModel->getId(), *word);

		cout << "done!" << endl;
	};

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



