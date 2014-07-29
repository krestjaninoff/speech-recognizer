#include <HmModel.h>
#include <ModelCommand.h>
#include "../audio/Processor.h"
#include "../model/Processor.h"
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

void ModelCommand::listLabels(Context& context) {
	const map<observation_t, CodeBookEntry*>* book = context.storage->getCodeBook()->getBook();
	map<observation_t, CodeBookEntry*>::const_iterator iter;

	cout << endl << "CodeBook:" << endl;
	for (iter = book->begin(); iter != book->end(); ++iter) {
		cout << "'" << *iter->first << "' (" << *iter->second->samplesCnt << "): [";

		MfccEntry* mfcc = *iter->second->avgVector;
		mfcc->print();
	}
}

void ModelCommand::addLabel(Context& context, const char* label) {

	cout << "Adding label '" << label << "' into the CodeBook..." << endl;
	cout << endl;

	observation_t observation = static_cast<observation_t>(label);

	// Read the observation data
	cout << "Enter MFCC vector elements (" << MFCC_SIZE << "): ";

	double* data = new double[MFCC_SIZE];
	double value;

	for (unsigned short i = 0; i < MFCC_SIZE; i++) {
		cin >> value;
		data[i] = value;
	}
	cout << endl;

	// Add or update the observation
	MfccEntry* mfcc = new MfccEntry(data, MFCC_SIZE);
	context.storage->addLabel(observation, mfcc);

	cout << "The observation was successfully added!" << endl;
}

void ModelCommand::removeLabel(Context& context, const char* label) {

	cout << "Deleting label '" << label << "' from the CodeBook...";

	observation_t observation = static_cast<observation_t>(label);
	context.storage->deleteLabel(observation);

	cout << " done!" << endl;
}

void ModelCommand::listModels(Context& context) {

	const map<uint32_t, HmModel*>* models = context.storage->getModels();

	if (models->size() > 0) {
		cout << "Available models are:" << endl;

		for (map<uint32_t, HmModel*>::const_iterator model = models->begin();
				model != models->end(); ++model) {

			cout << (*model).second->getId() << ": " << (*model).second->getText() << endl;
		}

	} else {
		cout << "There are no any models in the storage :( Use -a option add one!" << endl;
	}

	cout << endl;
}

void ModelCommand::displayModel(Context& context, const char* modelIdStr) {

	uint32_t modelId = static_cast<uint32_t>(modelIdStr);

	const map<uint32_t, HmModel*>* models = context.storage->getModels();
	HmModel* model = models[modelId];

	if (NULL != model) {
		model->print();

	} else {
		cout << "Model with id '" << modelIdStr << "' not found :(" << endl;
	}
}

void ModelCommand::addModel(Context& context, const char* modelNameChar) {

/*
	// TODO Check for unknown observations!!!

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
	SimpleModel* theModel = NULL;
	const map<uint32_t, SimpleModel*>* models = context.storage->getModels();
	for (map<uint32_t, SimpleModel*>::const_iterator model = models->begin();
			model != models->end(); ++model) {

		if (0 == strcmp(modelName.c_str(), (*model).second->getText().c_str())) {
			theModel = (*model).second;
			break;
		}
	}

	// Create the model if it does not exist
	string modelNameStr(modelName);
	if (NULL == theModel) {
		theModel = new SimpleModel(modelNameStr);
		context.storage->addModel(theModel);
	}

	// Add the sample to the model
	context.storage->addSample(theModel->getId(), *word);
	context.storage->persist();

	cout << "The new sample has been successfully added!" << endl;
*/
}


static void ModelCommand::trainModel(Context& context, const char* modelId) {
	// TODO Implement me
}


static void ModelCommand::deleteModel(Context& context, const char* modelId) {
	// TODO Implement me
}


string ModelCommand::doRecognize(Context& context, const char* modelNamesChar) {
/*
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
	vector<SimpleModel*>* modelsFiltered = new vector<SimpleModel*>();
	const map<uint32_t, SimpleModel*>* models = context.storage->getModels();

	vector<string> modelNames;
	if (NULL != modelNamesChar) {
		modelNames = parseString(modelNamesChar, ',');
	}

	for (map<uint32_t, SimpleModel*>::const_iterator model = models->begin();
			model != models->end(); ++model) {

		string modelName((*model).second->getText());
		if (0 == modelNames.size() ||
				find(modelNames.begin(), modelNames.end(), modelName) != modelNames.end()) {

			modelsFiltered->push_back((*model).second);
		}
	}

	// Try to recognize
	Recognizer rec(modelsFiltered);
	const SimpleModel* model = rec.recognize(*word);
	string theWord = "";

	// Print results
	if (NULL != model) {
		theWord =  model->getText();
	}

	return theWord;
*/
}

void ModelCommand::recognize(Context& context, const char* modelNamesChar) {
	string theWord = doRecognize(context, modelNamesChar);

	// Print results
	if (!theWord.empty()) {
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
	audio::Processor* processor = new audio::Processor(context.wavData);
	processor->init();
	context.processor = processor;

	cout << "Calculating MFCC for input data... " << endl;

	// Calc & show mfcc
	Word* word = processor->getAsWholeWord();
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
