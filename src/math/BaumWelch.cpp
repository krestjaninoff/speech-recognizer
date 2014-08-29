#include <assert.h>
#include <math.h>
#include <../config.h>
#include <Printer.h>
#include <BaumWelch.h>

using namespace std;
using namespace yazz::model;

namespace yazz {
namespace math {

/**
 * Max iteration count
 */
uint32_t BaumWelch::ITER_LIMIT = 50;
double BaumWelch::CONVERGENCE_EPSILON = 1e-4;

void BaumWelch::perform(HmModel* model, const vector<observation_t>* data) {

	// Check
	assert(data->size() > 0);

	// Model data
	size_t stateCnt = model->getStateCnt();
	size_t observationsCnt = model->getObservationCnt();
	observation_t* observations = model->getObservations();

	// Observations map
	map<observation_t, uint32_t> observMap;
	initObservationsMap(observMap, data, observations, observationsCnt);

	// Allocate memory
	double** a = new double*[stateCnt];
	for (size_t i = 0; i < stateCnt; i++) {
		a[i] = new double[data->size()];
	}

	double** b = new double*[stateCnt];
	for (size_t i = 0; i < stateCnt; i++) {
		b[i] = new double[data->size()];
	}

	double** y = new double*[stateCnt];
	for (size_t i = 0; i < stateCnt; i++) {
		y[i] = new double[data->size()];
	}

	double*** e = new double**[stateCnt];
	for (size_t i = 0; i < stateCnt; i++) {
		e[i] = new double*[stateCnt];

		for (size_t j = 0; j < stateCnt; j++) {
			e[i][j] = new double[data->size()];
		}
	}

	// Perform the algorithm
	algorithm(stateCnt, observationsCnt,
			model->getInitialDst(), model->getTransitions(), model->getEmissions(),
			data, observations, observMap, a, b, y, e);


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

void BaumWelch::algorithm(size_t stateCnt, size_t observationsCnt,
		double* initialDst, double** transitions, double** emissions,
		const vector<observation_t>* data, observation_t* observations,
		map<observation_t, uint32_t> observMap,
		double** a, double** b, double** y, double*** e) {

	double convergence = 0;
	uint32_t iter = 0;

	DEBUG("BaumWelch algorithm has started");
	do {
		DEBUG("Iteration: %d", iter);

		// Forward part (define "a")
		{
			uint32_t t = 0;
			for (size_t i = 0; i < stateCnt; i++) {
				observation_t observation = (*data)[0];
				a[i][t] = initialDst[i]
						* getObservProb(observation, i, emissions, observMap);
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
			}

			if (DEBUG_ENABLED) {
				cout << "A matrix is:" << endl;
				Printer::printMatrix(a, stateCnt, data->size());
				cout << endl;
			}
		}

		// Backward part (define "b")
		{
			uint32_t t = data->size() - 1;
			for (size_t i = 0; i < stateCnt; i++) {
				b[i][t] = 1;
			}

			vector<observation_t>::const_iterator iter = data->end();
			for (; t > 0; --t, iter--) {
				observation_t observation = *iter;

				for (size_t j = 0; j < stateCnt; j++) {

					double transitionSum = 0;
					for (size_t k = 0; k < stateCnt; k++) {

						transitionSum += b[k][t + 1] * transitions[j][k]
								* getObservProb(observation, j, emissions, observMap);
					}

					b[j][t] = transitionSum;
				}
			}

			if (DEBUG_ENABLED) {
				cout << "B matrix is:" << endl;
				Printer::printMatrix(b, stateCnt, data->size());
				cout << endl;
			}
		}

		// Calculate interim parameters: y and e
		{
			for (size_t i = 0; i < stateCnt; i++) {
				for (size_t t = 0; t < data->size(); t++) {
					double sum = 0;

					for (size_t j = 0; j < stateCnt; j++) {
						sum += a[j][t] * b[j][t];
					}

					y[i][t] = a[i][t] * b[i][t] / sum;
				}
			}

			if (DEBUG_ENABLED) {
				cout << "Y matrix is:" << endl;
				Printer::printMatrix(y, stateCnt, data->size());
				cout << endl;
			}

			for (size_t i = 0; i < stateCnt; i++) {
				for (size_t j = 0; j < stateCnt; j++) {

					for (size_t t = 0; t < data->size() - 1; t++) {
						double sum = 0;

						observation_t value = (*data)[t + 1];
						for (size_t k = 0; k < stateCnt; k++) {
							for (size_t l = 0; l < stateCnt; l++) {

								sum += a[k][t] * transitions[k][l] * b[l][t + 1]
										* getObservProb(value, l, emissions, observMap);
							}
						}

						e[i][j][t] = a[i][t] * transitions[i][j] * b[j][t + 1]
								* getObservProb(value, j, emissions, observMap)
								/ sum;
					}
				}
			}

			if (DEBUG_ENABLED) {
				cout << "E matrix is:" << endl;
				Printer::printMatrix3D(e, stateCnt, stateCnt, data->size());
				cout << endl;
			}
		}

		// Update the model
		{
			convergence = 0;

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
				cout << endl << "Convergence of initial distribution is: " << convergenceInitialDst;
				cout << endl;
			}

			// Transitions matrix
			double convergenceTransitions = 0;
			for (size_t i = 0; i < stateCnt; i++) {
				for (size_t j = 0; j < stateCnt; j++) {

					double sumY = 0;
					double sumE = 0;
					for (size_t t = 0; t < data->size() - 1; t++) {
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
				cout << "Transition matrix is:" << endl;
				Printer::printMatrix(transitions, stateCnt, stateCnt);
				cout << endl << "Convergence of transition matrix is: " << convergenceTransitions;
				cout << endl;
			}

			// Emissions matrix
			double convergenceEmissions = 0;
			for (size_t i = 0; i < stateCnt; i++) {

				double sumY = 0;
				for (size_t t = 0; t < data->size(); t++) {
					sumY += y[i][t];
				}

				for (size_t j = 0; j < observationsCnt; j++) {
					double sumYO = 0;

					for (size_t t = 0; t < data->size(); t++) {
						if (observations[j] == (*data)[t]) {
							sumYO = y[i][t];
						}
					}

					double emissionNewValue = sumYO / sumY;
					convergenceEmissions += fabs(emissionNewValue - emissions[i][j]);
					emissions[i][j] += emissionNewValue;
				}
			}
			convergenceEmissions /= stateCnt * observationsCnt;

			if (DEBUG_ENABLED) {
				cout << "Emission matrix is:" << endl;
				Printer::printMatrix(emissions, stateCnt, observationsCnt);
				cout << endl << "Convergence of emission matrix is: " << convergenceEmissions;
				cout << endl;
			}

			// Calculate common convergence
			convergence = (convergenceInitialDst + convergenceTransitions
					+ convergenceEmissions) / 3.;
		}

		if (DEBUG_ENABLED) {
			cout << "Iteration: " << iter << ", convergence: " << convergence << endl;
		}
		iter++;

	} while (convergence > CONVERGENCE_EPSILON && iter < ITER_LIMIT);

	assert(iter < ITER_LIMIT);
}

} /* namespace math */
} /* namespace yazz */
