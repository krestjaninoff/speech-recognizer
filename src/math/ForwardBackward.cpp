#include "../config.h"
#include <assert.h>
#include <Printer.h>
#include <ForwardBackward.h>

using namespace std;
using namespace yazz::model;

namespace yazz {
namespace math {

/**
 * Forward part (define "a")
 */
void ForwardBackward::forwardAlgorithm(size_t stateCnt, double* initialDst,
		double** transitions, double** emissions,
		const vector<observation_t>* sequence, double** a,
		map<observation_t, uint32_t>& observMap) {

	uint32_t t = 0;
	for (size_t i = 0; i < stateCnt; i++) {
		observation_t observation = (*sequence)[0];
		a[i][t] = initialDst[i]
				* getObservProb(observation, i, emissions, observMap);
	}

	vector<observation_t>::const_iterator iter = sequence->begin();
	for (t++, iter++; iter != sequence->end(); t++, iter++) {
		for (size_t j = 0; j < stateCnt; j++) {

			double transitionSum = 0;
			for (size_t k = 0; k < stateCnt; k++) {
				transitionSum += a[k][t - 1] * transitions[k][j];
			}

			observation_t observation = *iter;
			double probability = getObservProb(observation, j, emissions, observMap);

			a[j][t] = probability * transitionSum;
		}
	}

	if (DEBUG_ENABLED) {
		cout << "A matrix is:" << endl;
		Printer::printMatrix(a, stateCnt, sequence->size());
		cout << endl;
	}
}

/**
 * Backward part (define "b")
 */
void ForwardBackward::backwardAlgorithm(size_t stateCnt, double** transitions,
		double** emissions, const vector<observation_t>* sequence, double** b,
		map<observation_t, uint32_t>& observMap) {

	int32_t t = sequence->size() - 1;
	for (size_t i = 0; i < stateCnt; i++) {
		b[i][t] = 1;
	}

	vector<observation_t>::const_iterator iter = sequence->end();
	for (t--, iter--; t >= 0; t--, iter--) {

		observation_t observation = *iter;
		for (size_t j = 0; j < stateCnt; j++) {

			double transitionSum = 0;
			for (size_t k = 0; k < stateCnt; k++) {

				double probability = getObservProb(observation, k, emissions, observMap);
				transitionSum += b[k][t + 1] * transitions[j][k] * probability;
			}

			b[j][t] = transitionSum;
		}
	}

	if (DEBUG_ENABLED) {
		cout << "B matrix is:" << endl;
		Printer::printMatrix(b, stateCnt, sequence->size());
		cout << endl;
	}
}

double ForwardBackward::calcPossibility(const HmModel& model, const vector<observation_t>* sequence) {

	// Check
	assert(sequence->size() > 0);

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
		a[i] = new double[sequence->size()];
	}

	// Observations map
	map<observation_t, uint32_t> observMap;
	initObservationsMap(observMap, sequence, observations, observationsCnt);

	// Forward algorithm
	forwardAlgorithm(stateCnt,
		initialDst, transitions, emissions, sequence,
		a, observMap);

	// Calc probability
	double probability = 0.;
	size_t tMax = sequence->size() - 1;
	for (size_t i = 0; i < stateCnt; i++) {
		probability += a[i][tMax];
	}

	if (DEBUG_ENABLED) {
		cout << "Probability is: " << probability << endl << endl;
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
