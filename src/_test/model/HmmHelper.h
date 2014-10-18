#ifndef HMMHELPER_H_
#define HMMHELPER_H_

#include <HmModel.h>

using namespace yazz::model;

static HmModel* getModelOdin() {
	string text = "один";

	const vector<string> states = {"о", "д", "и", "н"};
	const vector<string> observations = {"а", "о", "д", "и", "ы", "н"};

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
	model->init(states, observations,
			transitions, emissions, initialDst, text);

	return model;
}

static HmModel* getModelDva() {
	string text = "два";

	const vector<string> states = {"д", "в", "а"};
	const vector<string> observations = {"д", "т", "в", "ф", "а", "о"};

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
	model->init(states, observations,
			transitions, emissions, initialDst, text);

	return model;
}

static HmModel* getModelTri() {
	string text = "три";

	const vector<string> states = {"т", "р", "и"};
	const vector<string> observations = {"т", "р", "и", "ы"};

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
	model->init(states, observations,
			transitions, emissions, initialDst, text);

	return model;
}

#endif /* HMMHELPER_H_ */
