#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include "../../config.h"
#include "../../math/BaumWelch.h"
#include "../model/HmmHelper.h"

using namespace yazz;
using namespace yazz::model;
using namespace yazz::math;

TEST(BAUM_WELCH, SIMPLE) {

	// Source model
	HmModel* model = getModelOdin();

	vector<observation_t> observations;
	observations.push_back('a');
	observations.push_back('d');
	observations.push_back('i');
	observations.push_back('n');

	// Model's attributes to check with
	double** transitions = new double*[4] {
		new double[4] {0.6, 0.4, 0.0, 0.0},
		new double[4] {0.0, 0.5, 5.0, 0.0},
		new double[4] {0.0, 0.0, 0.7, 0.3},
		new double[4] {0.0, 0.0, 0.0, 1.0}
	};
	double** emissions = new double*[4] {
		new double[6] {0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
		new double[6] {0.0, 0.0, 0.1, 0.0, 0.0, 0.0},
		new double[6] {0.0, 0.0, 0.0, 0.5, 0.5, 0.0},
		new double[6] {0.0, 0.0, 0.0, 0.0, 0.0, 1.0}
	};
	double* initialDst = new double[4] {1.0, 0.0, 0.0, 0.0};

	// Run the algorithm
	const vector<observation_t>* observationsConst = &observations;
	BaumWelch::perform(model, observationsConst);

	// Check results
	for (size_t i = 0; i < model->getStateCnt(); i++) {
		for (size_t j = 0; j < model->getStateCnt(); j++) {
			EXPECT_NEAR(model->getTransitions()[i][j], transitions[i][j], EPS_TEST);
		}
	}

	for (size_t i = 0; i < model->getStateCnt(); i++) {
		for (size_t j = 0; j < model->getObservationCnt(); j++) {
			EXPECT_NEAR(model->getEmissions()[i][j], emissions[i][j], EPS_TEST);
		}
	}

	for (size_t i = 0; i < model->getStateCnt(); i++) {
		EXPECT_NEAR(model->getInitialDst()[i], initialDst[i], EPS_TEST);
	}
}
