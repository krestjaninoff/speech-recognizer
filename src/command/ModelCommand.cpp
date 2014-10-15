#include <HmModel.h>
#include <ModelCommand.h>
#include <Storage.h>
#include <WavData.h>
#include <Word.h>
#include <Alphabet.h>
#include "../audio/Processor.h"
#include "../model/Processor.h"
#include <string.h>
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

void ModelCommand::listCodebook(Context& context) {
	const map<observation_t, CodeBookEntry*>* book = context.getStorage()->getCodeBook()->getBook();
	map<observation_t, CodeBookEntry*>::const_iterator iter;

	if (book->size() > 0) {
		cout << "CodeBook:" << endl;

		for (iter = book->begin(); iter != book->end(); ++iter) {
			cout << "'" << iter->first << "' (" << iter->second->samplesCnt << "): ";

			MfccEntry* mfcc = iter->second->avgVector;
			mfcc->print();
		}

	} else {
		cout << "There are no any observations in the storage :( See help to get know how to add one!" << endl;
	}
}

void ModelCommand::printCodebookEntry(Context& context, const char* observation) {

	const map<observation_t, CodeBookEntry*>* book = context.getStorage()->getCodeBook()->getBook();

	if (book->count(observation) > 0) {
		CodeBookEntry* entry = book->at(observation);

		cout << "Observation '" << observation << "' is based on " << entry->samplesCnt << " samples: ";
		entry->avgVector->print();
		cout << endl;

	} else {
		cout << "Observation with label '" << observation << "' not found :(" << endl;
	}
}

void ModelCommand::addCodebookEntry(Context& context, const char* observation) {
	string observationStr(observation);

	// Notice, that by adding an observation with the name which already exists in the Codebook
	// we just average its value. Hence, we don't need to check uniqueness of the observation.
	cout << "Adding label '" << observation << "' into the CodeBook..." << endl;
	cout << endl;

	// Read the observation data
	cout << "Enter MFCC vector elements (" << MFCC_SIZE << "): " << endl;

	double* data = new double[MFCC_SIZE];
	double value;

	for (unsigned short i = 0; i < MFCC_SIZE; i++) {

		cout << "[" << i + 1 << "]: ";
		cin >> value;

		data[i] = value;
	}
	cout << endl;

	// Add or update the observation
	MfccEntry* mfcc = new MfccEntry(data, MFCC_SIZE);
	context.getStorage()->addLabel(observationStr, mfcc);
	context.getStorage()->persist();

	cout << "The observation was successfully added!" << endl;
}

void ModelCommand::deleteCodebookEntry(Context& context, const char* observation) {

	const map<observation_t, CodeBookEntry*>* book = context.getStorage()->getCodeBook()->getBook();

	if (book->count(observation) > 0) {
		context.getStorage()->deleteLabel(observation);
		context.getStorage()->persist();

		cout << "Observation '" << observation << "' was deleted from the CodeBook" << endl;

	} else {
		cout << "Observation '" << observation << "' isn't in the CodeBook" << endl;
	}
}


void ModelCommand::listModels(Context& context) {

	const map<uint32_t, HmModel*>* models = context.getStorage()->getModels();

	if (models->size() > 0) {
		cout << "Available models are:" << endl;

		for (map<uint32_t, HmModel*>::const_iterator model = models->begin();
				model != models->end(); ++model) {

			cout << (*model).second->getId() << ": " << (*model).second->getText() << endl;
		}

	} else {
		cout << "There are no any models in the storage :( See help to get know how to add one!" << endl;
	}

	cout << endl;
}

void ModelCommand::printModel(Context& context, const char* modelIdStr) {

	uint32_t modelId = atoi(modelIdStr);
	const map<uint32_t, HmModel*>* models = context.getStorage()->getModels();

	if (models->count(modelId) > 0) {
		HmModel* model = models->at(modelId);

		cout << endl;
		model->print();

	} else {
		cout << "Model with id '" << modelIdStr << "' not found :(" << endl;
	}
}

// TODO Refactor states/observation + model input strategy
void ModelCommand::addModel(Context& context, const char* modelNameChar) {

	// Notice, that we can have a lot of models for the same value.
	// Hence, we don't need to check uniqueness of model's name
	cout << "Adding a new model... " << endl;

	// Check the model's name
	if (NULL == modelNameChar) {
		cout << "Model name is not specified" << endl;
		return;
	}
	string modelName(modelNameChar);

	// Read model's states
	size_t statesCnt;
	cout << "Enter amount of model's states: ";
	cin >> statesCnt;

	cout << "Enter model's states: " << endl;
	state_t* states = new state_t[statesCnt];
	for (size_t i = 0; i < statesCnt; i++) {
		cout << "[" << i << "]: ";
		cin >> states[i];
	}

	cout << endl;

	// Read model's observations
	size_t observationCnt;
	cout << "Enter amount of model's observations: ";
	cin >> observationCnt;

	cout << "Enter model's observations: " << endl;
	observation_t* observations = new observation_t[observationCnt];
	for (size_t i = 0; i < observationCnt; i++) {
		cout << "[" << i << "]: ";
		cin >> observations[i];
	}

	cout << endl;

	// Read model's transitions
	double** transitions = new double*[statesCnt];
	cout << "Enter model's transitions: " << endl;

	for (size_t i = 0; i < statesCnt; i++) {
		transitions[i] = new double[statesCnt];

		for (size_t j = 0; j < statesCnt; j++) {
			cout << "[" << i << "] " << states[i] << " -> " << "[" << j << "] " << states[j] << ": ";
			cin >> transitions[i][j];
		}

		cout << endl;
	}

	// Read model's emissions
	double** emissions = new double*[statesCnt];
	cout << "Enter model's emissions: " << endl;

	for (size_t i = 0; i < statesCnt; i++) {
		emissions[i] = new double[observationCnt];

		for (size_t j = 0; j < observationCnt; j++) {
			cout << "[" << i << "] " << observations[i] << " -> " << "[" << j << "] " << observations[j] << ": ";
			cin >> emissions[i][j];
		}

		cout << endl;
	}

	// Read model's initial distribution
	cout << "Enter model's initial distribution: " << endl;
	double* initialDist = new double[statesCnt];
	for (size_t i = 0; i < statesCnt; i++) {
		cout << "[" << i << "] " << states[i] << ": ";
		cin >> initialDist[i];
	}

	// Create and save the model
	HmModel* model = new HmModel();
	model->init(LETTERS, SOUNDS, transitions, emissions, initialDist, modelName);

	context.getStorage()->addModel(model);
	context.getStorage()->persist();

	cout << "The new sample has been successfully added!" << endl;
}

void ModelCommand::deleteModel(Context& context, const char* modelIdStr) {

	uint32_t modelId = atoi(modelIdStr);
	const map<uint32_t, HmModel*>* models = context.getStorage()->getModels();

	if (models->count(modelId) > 0) {

		context.getStorage()->deleteModel(modelId);
		context.getStorage()->persist();

		cout << "Model " << modelIdStr << " was successfully deleted" << endl;

	} else {
		cout << "Model " << modelIdStr << " doesn't exists" << endl;
	}
}

void ModelCommand::displayObservations(Context& context) {

    // Read MFCC data
    const vector<MfccEntry*>* data = getMfccData(context);

    // Transform MFCC to observations
    const vector<observation_t>* observations =
            context.getModelProcessor()->mfccToObservations(data);

    // Print results
    cout << endl << "Observations: ";
    for(vector<observation_t>::const_iterator iter = observations->begin();
            iter != observations->end(); iter++) {
        cout << *iter << " ";
    }
    cout << endl;

    // Clean up
    delete observations;
    for (vector<MfccEntry*>::const_iterator iter = data->begin(); iter != data->end(); ++iter) {
		delete *iter;
	}
    delete data;
}

void ModelCommand::trainModel(Context& context, const char* modelIdStr) {
	cout << "Training the model with Id " << modelIdStr;

	// Find the model
	uint32_t modelId = atoi(modelIdStr);
	HmModel* model = context.getStorage()->getModels()->at(modelId);

	if (NULL != model) {

		// Preparing training data
		const vector<MfccEntry*>* data = getMfccData(context);

		// Training
		cout << "Model before training:" << endl;
		model->print();

		model::Processor* processor = context.getModelProcessor();
		processor->trainModel(model, data);
		context.getStorage()->persist();

		cout << "Model after training:" << endl;
		model->print();

		// Clean up
		delete data;

	} else {
		cout << "Model not found!" << endl;
	}
}

string ModelCommand::doRecognize(Context& context, const char* modelNamesChar) {

	// Check the storage
	const map<uint32_t, HmModel*>* models = context.getStorage()->getModels();
	if (0 == models->size()) {
		cout << "Models storage is empty! Add some model before starting recognition." << endl;
		return NULL;
	}

	cout << "Word recognition... " << endl;

	// Get a word to recognize
	const vector<MfccEntry*>* data = getMfccData(context);

	// Get available models
	vector<HmModel*>* modelsFiltered = new vector<HmModel*>();

	vector<string> modelNames;
	if (NULL != modelNamesChar) {
		modelNames = parseString(modelNamesChar, ',');
	}

	for (map<uint32_t, HmModel*>::const_iterator model = models->begin();
			model != models->end(); ++model) {

		string modelName((*model).second->getText());
		if (0 == modelNames.size() ||
				find(modelNames.begin(), modelNames.end(), modelName) != modelNames.end()) {

			modelsFiltered->push_back((*model).second);
		}
	}
	const vector<HmModel*>* modelsToApply = modelsFiltered;

	// Recognition process
	model::Processor* processor = context.getModelProcessor();
	const HmModel* model = processor->findBestModel(modelsToApply, data);

	// Get the word
	string theWord = "";
	if (NULL != model) {
		theWord =  model->getText();
	}

	// Clean up
	delete data;

	return theWord;
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

const vector<MfccEntry*>* ModelCommand::getMfccData(Context& context) {

	// Check pre-requirements
	if (NULL == context.getWavData()) {
		cerr << "Input data is not specified :(" << endl;
		return NULL;
	}

	cout << "Checking input data... " << endl;

	// Create the Processor
	audio::Processor* processor = new audio::Processor(context.getWavData());
	processor->init();
	context.setAudioProcessor(processor);

	cout << "Calculating MFCC for input data... " << endl;

	// Calc & show mfcc
	const vector<Frame*>* frames = processor->getFrames();
	vector<MfccEntry*>* mfccData = new vector<MfccEntry*>();

	vector<Frame*>::const_iterator frame;
	for (frame = frames->begin(); frame != frames->end(); ++frame) {

		MfccEntry* entry = new MfccEntry((*frame)->getMFCC(), MFCC_SIZE);
		mfccData->push_back(entry);
	}

	return mfccData;
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
