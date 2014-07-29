#include "../config.h"
#include <DTW.h>
#include <Processor.h>
#include <iostream>
#include <iterator>
#include <string>

using namespace yazz::math;

namespace yazz {
namespace model {

Processor::Processor(Storage& storage) {
	this->storage = storage;
}

Processor::~Processor() {
	// Note, Storage is an independent object, Processor doesn't own it.
}

void Processor::trainModel(HmModel* model, vector<MfccEntry*>* data) {
	vector<observation_t>* observations = this->mfccToLabels(data);

	// TODO Implement Baum-Welch algorithm for "model" and "data"

	delete observations;
}

HmModel* Processor::findBestModel(const vector<MfccEntry*>* data) {

	HmModel* bestModel = NULL;
	double minDistance = 0.;

	const map<uint32_t, HmModel*>* models = this->storage.getModels();
	for (map<uint32_t, HmModel*>::const_iterator iter = models->begin();
			iter != models->end(); ++iter) {

		HmModel* model = *iter->second;
		double probability = 0.;
		// TODO Implement EM algorithm for "model" and "data"

		cout << "Probability for model \"" << model->getText().c_str() << "\" is " << probability << endl;

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
	const CodeBook* codeBook = this->storage.getCodeBook();

	cout << "Labels:" << endl;

	vector<MfccEntry*>::const_iterator iter;
	for (iter = mfcc->begin(); iter != mfcc->end(); ++model) {
		MfccEntry* entry = *iter;
		observation_t label = codeBook->findLabelBySample(entry);

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
