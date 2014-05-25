#include "DTW.h"
#include "Recognizer.h"

namespace wtm {
namespace math {

	Recognizer::Recognizer(vector<Model*>* models) {
		this->models = models;
	}

	Recognizer::~Recognizer() {
		delete this->models;
	}

	const Model* Recognizer::recognize(const Word& word) {
		Model* bestModel = NULL;
		double minDistance = 0.;

		for (vector<Model*>::const_iterator model = this->models->begin();
				model != this->models->end(); ++model) {

			double distance = 0.;
			for (vector<MFCCSample>::const_iterator sample = (*model)->getSamples().begin();
					sample != (*model)->getSamples().end(); ++sample) {

				distance += DTW::calcDistance(word.getMfcc(), word.getMfccSize(),
						(*sample).data, (*sample).size);
			}
			distance /= (*model)->getSamples().size();

			DEBUG("Distance for model is %s is %f", (*model)->getText().c_str(), distance);

			if (NULL == bestModel || distance < minDistance) {
				minDistance = distance;
				bestModel = *model;
			}
		}

		DEBUG("Best model %s with %f distance", bestModel->getText().c_str(), minDistance);

		return bestModel;
	}

} /* namespace math */
} /* namespace wtm */
