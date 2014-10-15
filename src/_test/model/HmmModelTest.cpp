#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include "HmmHelper.h"

TEST(HMM_MODEL, SERIALISATION) {

	HmModel* modelOriginal = getModelOdin();
	HmModel* modelDecoded = new HmModel();

	stringstream stream;
	stream << *modelOriginal;
	stream >> *modelDecoded;

	EXPECT_EQ(modelOriginal->getStateCnt(), modelDecoded->getStateCnt());
	EXPECT_EQ(modelOriginal->getStates()[0], modelDecoded->getStates()[0]);
	EXPECT_EQ(modelOriginal->getObservationCnt(), modelDecoded->getObservationCnt());
	EXPECT_EQ(modelOriginal->getObservations()[0], modelDecoded->getObservations()[0]);
}
