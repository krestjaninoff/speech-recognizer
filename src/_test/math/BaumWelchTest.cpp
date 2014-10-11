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
	observations.push_back('6');
	observations.push_back('3');
	observations.push_back('2');
	observations.push_back('3');
	observations.push_back('3');
	observations.push_back('4');
	observations.push_back('4');
	observations.push_back('6');
	observations.push_back('6');
	observations.push_back('4');
	observations.push_back('3');
	observations.push_back('5');

	// Model's attributes to check with
	double** transitions = new double*[5] {
		new double[5] {0.8718, 0.0252, 0.0219, 0.0336, 0.0475},
		new double[5] {0.0258, 0.8728, 0.0217, 0.0314, 0.0483},
		new double[5] {0.0306, 0.0280, 0.8489, 0.0349, 0.0576},
		new double[5] {0.0214, 0.0203, 0.0194, 0.9008, 0.0381},
		new double[5] {0.0143, 0.0147, 0.0141, 0.0195, 0.9374}
	};
	double** emissions = new double*[5] {
		new double[6] {0.0000, 0.1043, 0.3663, 0.2274, 0.0741, 0.2280},
		new double[6] {0.0000, 0.0942, 0.3376, 0.2308, 0.0680, 0.2694},
		new double[6] {0.0000, 0.0796, 0.3201, 0.2168, 0.1156, 0.2680},
		new double[6] {0.0000, 0.0594, 0.2889, 0.2845, 0.1027, 0.2645},
		new double[6] {0.0000, 0.0810, 0.3397, 0.2616, 0.0779, 0.2399}
	};
	double* initialDst = new double[5] {0.1789,	0.2195,	0.1230,	0.1262,	0.3524};

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
