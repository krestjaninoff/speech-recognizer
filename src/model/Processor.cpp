#include "../config.h"
#include <DTW.h>
#include <Processor.h>
#include <iostream>
#include <iterator>
#include <string>

using namespace yazz::math;

namespace yazz {
namespace model {

Processor::Processor(map<string&, HmModel*>* models, CodeBook* codeBook) {
	this->models = models;
	this->codeBook = codeBook;
}

Processor::~Processor() {
	if (NULL != this->models) {
		map<string&, HmModel*>::const_iterator iter;
		for (iter = this->models->begin(); iter != this->models->end(); ++iter) {
			delete *iter->second;
		}

		delete this->models;
	}

	if (NULL != this->codeBook) {
		delete codeBook;
	}
}

HmModel* Processor::getModelByText(const string& text) {
	return this->models[text];
}

void Processor::trainModel(HmModel* model, vector<MfccEntry*>* data) {
	vector<observation_t>* observations = this->mfccToLabels(data);

	// TODO Implement Baum-Welch algorithm for "model" and "data"

	delete observations;
}

HmModel* Processor::findBestModel(const vector<MfccEntry*>* data) {

	HmModel* bestModel = NULL;
	double minDistance = 0.;

	for (vector<HmModel*>::const_iterator model = this->models->begin();
			model != this->models->end(); ++model) {

		double probability = 0.;
		// TODO Implement EM algorithm for "model" and "data"

		cout << "Probability for model \"" << (*model)->getText().c_str() << "\" is " << probability << endl;

		if (NULL == bestModel || probability < minDistance) {
			minDistance = probability;
			bestModel = *model;
		}
	}

	cout << "The best model is \"" << bestModel->getText().c_str() << "\" with " <<
			minDistance << " distance" << endl;

	return bestModel;

	return NULL;
}

const vector<observation_t>* Processor::mfccToLabels(const vector<MfccEntry*>* mfcc) {
	vector<observation_t>* labels = new vector<observation_t>*();

	cout << "Labels:" << endl;

	vector<MfccEntry*>::const_iterator iter;
	for (iter = mfcc->begin(); iter != mfcc->end(); ++model) {
		MfccEntry* entry = *iter;
		observation_t label = this->codeBook->findLabelBySample(entry);

		labels->push_back(label);

		cout << label << ": ";
		for (size_t i = 0; i < entry->getSize(); i++) {
			cout << entry->getData()[i] << ", ";
		}
		cout << endl;
	}

	cout << endl;

	return NULL;
}

} /* namespace model */
} /* namespace yazz */
