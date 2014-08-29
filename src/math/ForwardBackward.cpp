#include "../config.h"
#include <assert.h>
#include <Printer.h>
#include <ForwardBackward.h>

using namespace std;
using namespace yazz::model;

namespace yazz {
namespace math {

double ForwardBackward::forward(const HmModel& model, const vector<observation_t>* data) {

	// Check
	assert(data->size() > 0);

	// Model data
	size_t stateCnt = model.getStateCnt();
	size_t observationsCnt = model.getObservationCnt();
	observation_t* observations = model.getObservations();
	double* initialDst = model.getInitialDst();
	double** transitions = model.getTransitions();
	double** emissions = model.getEmissions();

	// Memory allocation
	double** a = new double*[stateCnt];
	for (size_t i = 0; i < stateCnt; i++) {
		a[i] = new double[data->size()];
	}

	// Observations map
	map<observation_t, uint32_t> observMap;
	initObservationsMap(observMap, data, observations, observationsCnt);

	uint32_t t = 0;
	for (size_t i = 0; i < stateCnt; i++) {
		observation_t observation = (*data)[0];
		a[i][t] = initialDst[i]
				* getObservProb(observation, i, emissions, observMap);
	}

	if (DEBUG_ENABLED) {
		cout << "Iteration " << t << ":" << endl;
		Printer::printMatrix(a, stateCnt, data->size());
		cout << endl;
	}

	vector<observation_t>::const_iterator iter = data->begin();
	for (; iter != data->end(); ++t, ++iter) {
		for (size_t j = 1; j < stateCnt; j++) {

			double transitionSum = 0;
			for (size_t k = 0; k < stateCnt; k++) {
				transitionSum += a[k][t - 1] * transitions[k][j];
			}

			observation_t observation = *iter;
			a[j][t] = getObservProb(observation, j, emissions, observMap)
					* transitionSum;
		}

		if (DEBUG_ENABLED) {
			cout << "Iteration " << t << ":" << endl;
			cout << endl;
			Printer::printMatrix(a, stateCnt, data->size());
			cout << endl;
		}
	}

	double probability = 0.;
	size_t tMax = data->size() - 1;
	for (size_t i = 0; i < stateCnt; i++) {
		probability += a[i][tMax];
	}

	if (DEBUG_ENABLED) {
		cout << "Probability is: " << probability << endl;
	}

	// Clean up
	for (size_t i = 0; i < stateCnt; i++) {
		delete [] a[i];
	}
	delete [] a;

	return probability;
}

} /* namespace math */
} /* namespace yazz */
