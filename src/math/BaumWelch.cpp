#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <limits>
#include <../config.h>
#include <Printer.h>
#include <BaumWelch.h>
#include <ForwardBackward.h>

using namespace std;
using namespace yazz::model;

namespace yazz {
namespace math {

/**
 * Max iteration count
 */
uint32_t BaumWelch::ITER_LIMIT = 50;
double BaumWelch::CONVERGENCE_EPSILON = 1e-3;

void BaumWelch::perform(HmModel* model, const vector<observation_t>* sequence) {

	// Check
	assert(sequence->size() > 0);

	// Model data
	size_t stateCnt = model->getStateCnt();
	size_t observationsCnt = model->getObservationCnt();
	observation_t* observations = model->getObservations();

	// Observations map
	map<observation_t, uint32_t> observMap;
	initObservationsMap(observMap, sequence, observations, observationsCnt);

	// Allocate memory
	double** a = new double*[stateCnt];
	for (size_t i = 0; i < stateCnt; i++) {
		a[i] = new double[sequence->size()];
		memset(a[i], 0, sequence->size());
	}

	double** b = new double*[stateCnt];
	for (size_t i = 0; i < stateCnt; i++) {
		b[i] = new double[sequence->size()];
		memset(b[i], 0, sequence->size());
	}

	double** y = new double*[stateCnt];
	for (size_t i = 0; i < stateCnt; i++) {
		y[i] = new double[sequence->size()];
		memset(y[i], 0, sequence->size());
	}

	double*** e = new double**[stateCnt];
	for (size_t i = 0; i < stateCnt; i++) {
		e[i] = new double*[stateCnt];

		for (size_t j = 0; j < stateCnt; j++) {
			e[i][j] = new double[sequence->size()];
			memset(e[i][j], 0, sequence->size());
		}
	}

	// Perform the algorithm
	algorithm(stateCnt, observationsCnt,
			model->getInitialDst(), model->getTransitions(), model->getEmissions(),
			sequence, observations, observMap, a, b, y, e);


	// Clean up
	for (size_t i = 0; i < stateCnt; i++) {
		for (size_t j = 0; j < stateCnt; j++) {
			delete [] e[i][j];
		}
		delete [] e[i];
	}
	delete [] e;

	for (size_t i = 0; i < stateCnt; i++) {
		delete [] y[i];
	}
	delete [] y;

	for (size_t i = 0; i < stateCnt; i++) {
		delete [] b[i];
	}
	delete [] b;

	for (size_t i = 0; i < stateCnt; i++) {
		delete [] a[i];
	}
	delete [] a;
}

void BaumWelch::calculateGamma(size_t stateCnt,
		const vector<observation_t>* sequence, double** a, double** b,
		double** y) {

	for (size_t i = 0; i < stateCnt; i++) {
		for (size_t t = 0; t < sequence->size(); t++) {

			double sum = 0;
			for (size_t j = 0; j < stateCnt; j++) {
				sum += a[j][t] * b[j][t];
			}

			y[i][t] = a[i][t] * b[i][t] / sum;
		}
	}

	if (DEBUG_ENABLED) {
		cout << "Y matrix is:" << endl;
		Printer::printMatrix(y, stateCnt, sequence->size());
		cout << endl;
	}
}

void BaumWelch::calculateXi(size_t stateCnt, double** transitions,
		double** emissions, const vector<observation_t>* sequence,
		map<observation_t, uint32_t>& observMap, double** a, double** b,
		double*** e) {

	for (size_t i = 0; i < stateCnt; i++) {
		for (size_t j = 0; j < stateCnt; j++) {

			for (size_t t = 0; t < sequence->size() - 1; t++) {
				double sum = 0;

				observation_t value = (*sequence)[t + 1];
				for (size_t k = 0; k < stateCnt; k++) {

					for (size_t l = 0; l < stateCnt; l++) {

						double probability = getObservProb(value, l, emissions, observMap);
						sum += a[k][t] * transitions[k][l] * b[l][t + 1] * probability;
					}
				}

				double probability = getObservProb(value, j, emissions,	observMap);
				e[i][j][t] = a[i][t] * transitions[i][j] * b[j][t + 1] * probability / sum;
			}
		}
	}

	if (DEBUG_ENABLED) {
		cout << "E matrix is:" << endl;
		Printer::printMatrix3D(e, stateCnt, stateCnt, sequence->size());
		cout << endl;
	}
}

double BaumWelch::updateModel(size_t stateCnt, size_t observationsCnt,
		double* initialDst, double** transitions, double** emissions,
		const vector<observation_t>* sequence,
		observation_t* observations, map<observation_t, uint32_t>& observMap,
		double** a, double** b,	double** y, double*** e) {
	double convergence = 0.;

	// Initial distribution
	double convergenceInitialDst = 0;
	for (size_t i = 0; i < stateCnt; i++) {

		convergenceInitialDst += fabs(y[i][0] - initialDst[i]);
		initialDst[i] = y[i][0];
	}
	convergenceInitialDst /= stateCnt;

	if (DEBUG_ENABLED) {
		cout << "Initial distributions is:" << endl;
		Printer::printVector(initialDst, stateCnt);
		cout << "Convergence of initial distribution is: " << convergenceInitialDst;
		cout << endl;
	}

	// Transitions matrix
	double convergenceTransitions = 0;
	for (size_t i = 0; i < stateCnt; i++) {
		for (size_t j = 0; j < stateCnt; j++) {

			double sumY = 0;
			double sumE = 0;
			for (size_t t = 0; t < sequence->size() - 1; t++) {
				sumY += y[i][t];
				sumE += e[i][j][t];
			}

			double transitionNewValue = sumE / sumY;
			convergenceTransitions += fabs(	transitionNewValue - transitions[i][j]);
			transitions[i][j] = transitionNewValue;
		}
	}
	convergenceTransitions /= stateCnt * stateCnt;

	if (DEBUG_ENABLED) {
		cout << endl << "Transition matrix is:" << endl;
		Printer::printMatrix(transitions, stateCnt, stateCnt);
		cout << "Convergence of transition matrix is: " << convergenceTransitions;
		cout << endl;
	}

	// Emissions matrix
	double convergenceEmissions = 0;
	for (size_t i = 0; i < stateCnt; i++) {

		double sumY = 0;
		for (size_t t = 0; t < sequence->size(); t++) {
			sumY += y[i][t];
		}

		for (size_t j = 0; j < observationsCnt; j++) {

			double sumYO = 0;
			for (size_t t = 0; t < sequence->size(); t++) {
				if (observations[j] == (*sequence)[t]) {
					sumYO += y[i][t];
				}
			}

			double emissionNewValue = sumYO / sumY;
			convergenceEmissions += fabs(emissionNewValue - emissions[i][j]);
			emissions[i][j] = emissionNewValue;
		}
	}
	convergenceEmissions /= stateCnt * observationsCnt;

	if (DEBUG_ENABLED) {
		cout << endl << "Emission matrix is:" << endl;
		Printer::printMatrix(emissions, stateCnt, observationsCnt);
		cout << "Convergence of emission matrix is: " << convergenceEmissions;
		cout << endl;
	}

	// Calculate common convergence
	convergence = (convergenceInitialDst + convergenceTransitions
			+ convergenceEmissions) / 3.;

	return convergence;
}

void BaumWelch::algorithm(size_t stateCnt, size_t observationsCnt,
		double* initialDst, double** transitions, double** emissions,
		const vector<observation_t>* sequence,
		observation_t* observations, map<observation_t, uint32_t> observMap,
		double** a, double** b, double** y, double*** e) {

	double convergence = 0;
	uint32_t iter = 0;

	DEBUG("BaumWelch algorithm has started");
	do {
		if (DEBUG_ENABLED) {
			cout << endl << "Iteration: " << iter << endl << endl;
		}

		// Forward part (define "a")
		ForwardBackward::forwardAlgorithm(stateCnt,
				initialDst, transitions, emissions, sequence,
				a, observMap);

		// Backward part (define "b")
		ForwardBackward::backwardAlgorithm(stateCnt,
				transitions, emissions, sequence,
				b, observMap);

		// Calculate interim parameters: y and e
		{
			calculateGamma(stateCnt, sequence, a, b, y);

			calculateXi(stateCnt, transitions, emissions, sequence,
					observMap, a, b, e);
		}

		// Update the model
		convergence = updateModel(stateCnt, observationsCnt,
				initialDst, transitions, emissions,
				sequence,
				observations, observMap,
				a, b, y, e);

		if (DEBUG_ENABLED) {
			cout << "Total convergence: " << convergence << endl << endl;
		}

		iter++;
		// char answer; cout << "Next iteration?"; cin >> answer;

	} while (convergence > CONVERGENCE_EPSILON && iter < ITER_LIMIT);

	assert(iter < ITER_LIMIT);
}

} /* namespace math */
} /* namespace yazz */
