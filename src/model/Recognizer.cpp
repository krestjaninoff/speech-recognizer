#include <DTW.h>
#include <Recognizer.h>
#include <iostream>
#include <iterator>

namespace yazz {
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

			cout << "Distance for model \"" << (*model)->getText().c_str() << "\" is " << distance << endl;

			if (NULL == bestModel || distance < minDistance) {
				minDistance = distance;
				bestModel = *model;
			}
		}

		cout << "The best model is \"" << bestModel->getText().c_str() << "\" with " << minDistance << " distance" << endl;;

		return bestModel;
	}

} /* namespace math */
} /* namespace yazz */
