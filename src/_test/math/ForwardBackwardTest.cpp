#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include "../../math/ForwardBackward.h"
#include "../model/HmmHelper.h"

using namespace std;
using namespace yazz::model;
using namespace yazz::math;

TEST(FORWARD_BACKWARD, FORWARD) {

	const HmModel& modelOdin = *getModelOdin();
	const HmModel& modelDva = *getModelDva();
	const HmModel& modelTri = *getModelTri();

	vector<observation_t> observations;
	observations.push_back("т");
	observations.push_back("в");
	observations.push_back("а");
	const vector<observation_t>* observationsConst = &observations;

	double probabilityOdin = ForwardBackward::calcPossibility(modelOdin, observationsConst);
	double probabilityDva = ForwardBackward::calcPossibility(modelDva, observationsConst);
	double probabilityTri = ForwardBackward::calcPossibility(modelTri, observationsConst);

	double probabilityMax = max(max(probabilityOdin, probabilityDva), probabilityTri);

	EXPECT_EQ(probabilityDva, probabilityMax);

	delete &modelOdin;
	delete &modelDva;
	delete &modelTri;
}
