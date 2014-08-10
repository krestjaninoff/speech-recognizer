#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include "../../math/ForwardBackward.h"
#include "../model/HmmHelper.h"

using namespace yazz::model;
using namespace yazz::math;

TEST(FORWARD_BACKWARD, SIMPLE) {

	vector<HmModel*> models;
	models.push_back(getModelOdin());
	models.push_back(getModelDva());
	models.push_back(getModelTri());

	vector<observation_t> observations;
	observations.push_back('t');
	observations.push_back('w');
	observations.push_back('a');

	const vector<HmModel*>* modelsConst = &models;
	const vector<observation_t>* observationsConst = &observations;
	const HmModel* model = ForwardBackward::perform(modelsConst, observationsConst);

	EXPECT_TRUE(NULL != model);
	EXPECT_EQ(model->getText(), "dva");

	while (!models.empty()) {
		delete models.back(), models.pop_back();
	}
}
