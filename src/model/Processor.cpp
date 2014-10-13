#include "../config.h"
#include <iostream>
#include <iterator>
#include <string>
#include "../math/Basic.h"
#include "../math/ForwardBackward.h"
#include "../math/BaumWelch.h"
#include "Processor.h"

using namespace yazz::math;

namespace yazz {
namespace model {

Processor::Processor(Storage* storage) {
	this->storage = storage;
}

Processor::~Processor() {
	// Note, Storage is an independent object, Processor doesn't own it.
}

void Processor::trainModel(HmModel* model, const vector<MfccEntry*>* data) {
	const vector<observation_t>* observations = this->mfccToObservations(data);

	BaumWelch::perform(model, observations);

	delete observations;
}

const HmModel* Processor::findBestModel(const vector<HmModel*>* models,
		const vector<MfccEntry*>* data) {
	const vector<observation_t>* observations = this->mfccToObservations(data);

	const HmModel* bestModel = NULL;
	double minProbability = 0.;

	for (vector<HmModel*>::const_iterator iter = models->begin();
			iter != models->end(); ++iter) {

		const HmModel& model = **iter;
		double probability = 0.;

		probability = ForwardBackward::calcPossibility(model, observations);

		cout << "Probability for model \"" << model.getText().c_str() << "\" is " << probability << endl;

		if (NULL == bestModel || probability < minProbability) {
			minProbability = probability;
			bestModel = &model;
		}
	}

	cout << "The best model is \"" << bestModel->getText().c_str() << "\" with " <<
			minProbability << " distance" << endl;

	return bestModel;
}

const vector<observation_t>* Processor::mfccToObservations(const vector<MfccEntry*>* mfcc) {
	vector<observation_t>* observations = new vector<observation_t>();
	const CodeBook* codeBook = this->storage->getCodeBook();

	if (DEBUG_ENABLED) {
		cout << "Labels:" << endl;
	}

	vector<MfccEntry*>::const_iterator iter;
	for (iter = mfcc->begin(); iter != mfcc->end(); ++iter) {
		MfccEntry* entry = *iter;
		observation_t label = codeBook->findLabelBySample(entry);

		observations->push_back(label);

		if (DEBUG_ENABLED) {
			cout << label << ": ";
			for (size_t i = 0; i < entry->getSize(); i++) {
				cout << entry->getData()[i] << ", ";
			}
			cout << endl;
		}
	}

	return observations;
}

} /* namespace model */
} /* namespace yazz */
