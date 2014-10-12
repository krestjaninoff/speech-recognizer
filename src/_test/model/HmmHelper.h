#ifndef HMMHELPER_H_
#define HMMHELPER_H_

#include <HmModel.h>

using namespace yazz::model;

static HmModel* getModelOdin() {
	string text = "odin";

	size_t stateCnt = 4;
	state_t* states = new state_t[4] {'o', 'd', 'i', 'n'};

	size_t observationCnt = 6;
	observation_t* observations = new observation_t[6] {'a', 'o', 'd', 'i', 'y', 'n'};

	double** transitions = new double*[4] {
		new double[4] {0.6, 0.4, 0.0, 0.0},
		new double[4] {0.0, 0.5, 0.5, 0.0},
		new double[4] {0.0, 0.0, 0.7, 0.3},
		new double[4] {0.0, 0.0, 0.0, 1.0}
	};
	double** emissions = new double*[4] {
		new double[6] {0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
		new double[6] {0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
		new double[6] {0.0, 0.0, 0.0, 0.5, 0.5, 0.0},
		new double[6] {0.0, 0.0, 0.0, 0.0, 0.0, 1.0}
	};

	double* initialDst = new double[4] {1.0, 0.0, 0.0, 0.0};

	HmModel* model = new HmModel();
	model->init(states, stateCnt, observations, observationCnt,
			transitions, emissions, initialDst, text);

	return model;
}

static HmModel* getModelDva() {
	string text = "dva";

	size_t stateCnt = 3;
	state_t* states = new state_t[3] {'d', 'v', 'a'};

	size_t observationCnt = 6;
	observation_t* observations = new observation_t[6] {'d', 't', 'v', 'w', 'a', 'r'};

	double** transitions = new double*[3] {
		new double[3] {0.5, 0.5, 0.0},
		new double[3] {0.0, 0.7, 0.3},
		new double[3] {0.0, 0.0, 1.0}
	};
	double** emissions = new double*[3] {
		new double[6] {0.8, 0.2, 0.0, 0.0, 0.0, 0.0},
		new double[6] {0.0, 0.0, 0.5, 0.5, 0.0, 0.0},
		new double[6] {0.0, 0.0, 0.0, 0.0, 0.8, 0.2}
	};

	double* initialDst = new double[3] {0.9, 0.1, 0.0};

	HmModel* model = new HmModel();
	model->init(states, stateCnt, observations, observationCnt,
			transitions, emissions, initialDst, text);

	return model;
}

static HmModel* getModelTri() {
	string text = "tri";

	size_t stateCnt = 3;
	state_t* states = new state_t[3] {'t', 'r', 'i'};

	size_t observationCnt = 4;
	observation_t* observations = new observation_t[4] {'t', 'r', 'i', 'y'};

	double** transitions = new double*[3] {
		new double[3] {0.2, 0.8, 0.0},
		new double[3] {0.0, 0.7, 0.3},
		new double[3] {0.0, 0.0, 1.0}
	};
	double** emissions = new double*[3] {
		new double[4] {1.0, 0.0, 0.0, 0.0},
		new double[4] {0.0, 1.0, 0.0, 0.0},
		new double[4] {0.0, 0.0, 0.7, 0.3}
	};

	double* initialDst = new double[3] {1.0, 0.0, 0.0};

	HmModel* model = new HmModel();
	model->init(states, stateCnt, observations, observationCnt,
			transitions, emissions, initialDst, text);

	return model;
}

#endif /* HMMHELPER_H_ */
