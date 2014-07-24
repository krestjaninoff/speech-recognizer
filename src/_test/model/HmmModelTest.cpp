#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include <HmmModel.h>

using namespace yazz::model;

TEST(HMM_MODEL, READ_WRITE)
{
	string text = "dva";

	size_t stateCnt = 3;
	state_t* states = new state_t[3] {'a', 'd', 'v'};

	size_t observationCnt = 6;
	observation_t* observations = new observation_t[6] {'a', 'r', 'd', 't', 'v', 'w'};

	double** transitions = new double*[3] {
		new double[3] {0.0, 0.1, 0.9},
		new double[3] {0.7, 0.3, 0.0},
		new double[3] {0.0, 0.0, 1.0}
	};
	double** emissions = new double*[3] {
		new double[6] {0.8, 0.8, 0.5, 0.5, 0.3, 0.3},
		new double[6] {0.8, 0.8, 0.5, 0.5, 0.3, 0.3},
		new double[6] {0.8, 0.8, 0.5, 0.5, 0.3, 0.3}
	};

	double* initialDst = new double[3] {0., 1., 0.};

	HmmModel* modelOriginal = new HmmModel();
	modelOriginal->init(states, stateCnt, observations, observationCnt,
			transitions, emissions, initialDst, text);

	stringstream stream;
	HmmModel* modelDecoded = new HmmModel();

	stream << *modelOriginal;
	stream >> *modelDecoded;

	EXPECT_EQ(modelOriginal->getStateCnt(), modelDecoded->getStateCnt());
	EXPECT_EQ(modelOriginal->getObservationCnt(), modelDecoded->getObservationCnt());
}
