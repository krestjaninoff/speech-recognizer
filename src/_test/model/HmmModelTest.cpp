#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include "HmmHelper.h"

TEST(HMM_MODEL, READ_WRITE) {

	HmModel* modelOriginal = getModelOdin();
	HmModel* modelDecoded = new HmModel();

	stringstream stream;
	stream << *modelOriginal;
	stream >> *modelDecoded;

	EXPECT_EQ(modelOriginal->getStateCnt(), modelDecoded->getStateCnt());
	EXPECT_EQ(modelOriginal->getObservationCnt(), modelDecoded->getObservationCnt());
}
