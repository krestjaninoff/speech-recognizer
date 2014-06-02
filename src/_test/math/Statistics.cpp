#include "../math/Statistics.h"
#include "gtest/gtest.h"

using namespace yazz::math;

TEST(MATH_STATISTICS, RMS)
{
	double f[] = {2, 4, 3, 1};
	double rms = Statistics::rms(f, 0, 3);

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
	double f[] = {2, 4, 3, 1, 6, 3, 9, 2, 3, 6, 1, 7};
	uint8_t binCnt = 4;
	uint32_t min = 1;
	uint32_t max = 9;

	double entropy = Statistics::entropy(f, 0, 11, binCnt, min, max);

	EXPECT_NEAR(1.9183, entropy, 0.001);
}

