#include <Recognizer.h>

namespace wtm {
namespace math {

	Recognizer::Recognizer(vector<Model>* models) {
		this->models = models;
	}

	Recognizer::~Recognizer() {
		delete this->models;
	}

	const Model* Recognizer::recognize(const Word& word) {
		Model* bestModel = NULL;
		double minDistance = 0.;

		for (vector<Model>::const_iterator model = this->models->begin();
				model != this->models->end(); ++model) {

			double distance = 0.;
			for (vector<MFCCSample>::const_iterator sample = *model->samples.begin();
					sample != *model->samples->end(); ++sample) {

				distance += DTW::calcDistance(word.mfcc, word.mfccSize,
						*sample->data, *sample->size);
			}
			distance /= *model->getSamples().size();

			DEBUG("Distance for model is %s is %d", *model->getText(), distance);

			if (NULL == bestModel || distance < minDistance) {
				minDistance = distance;
				bestModel = *model;
			}
		}

		DEBUG("Best model %s with %d distance", bestModel->getText(), minDistance);

		return bestModel;
	}

} /* namespace math */
} /* namespace wtm */
