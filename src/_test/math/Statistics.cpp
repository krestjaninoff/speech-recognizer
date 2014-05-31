#include "../math/Statistics.h"
#include "gtest/gtest.h"

using namespace yazz::math;

TEST(MATH_STATISTICS, RMS)
{
	vector<raw_t> data;
	data.push_back(2);
	data.push_back(4);
	data.push_back(3);
	data.push_back(1);

	const vector<raw_t>& dataRef = data;
	double rms = Statistics::rms(dataRef, 0, data.size() - 1);

	ASSERT_DOUBLE_EQ(2.7386127875258306, rms);
}


/**
 * Matlab/Octave implementation
 *
 * I = [2,4,3,1,6,3,8,2,3,6,1,7]
 * NBin =  4
 * P = hist (I(:), NBin, true)
 * P += (P == 0);
 * retval = -sum (P .* log2 (P));
 */
TEST(MATH_STATISTICS, ENTROPY)
{
	vector<raw_t> data;
	data.push_back(2);
	data.push_back(4);
	data.push_back(3);
	data.push_back(1);
	data.push_back(6);
	data.push_back(3);
	data.push_back(9);
	data.push_back(2);
	data.push_back(3);
	data.push_back(6);
	data.push_back(1);
	data.push_back(7);

	uint8_t binCnt = 4;
	uint32_t min = 1;
	uint32_t max = 9;

	const vector<raw_t>& dataRef = data;
	double entropy = Statistics::entropy(dataRef, 0, data.size() - 1, binCnt, min, max);

	EXPECT_NEAR(1.9183, entropy, 0.001);
}

