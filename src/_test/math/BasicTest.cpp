#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include "../config.h"
#include <Basic.h>

using namespace yazz;
using namespace yazz::math;

TEST(MATH_BASIC, RMS) {

	raw_t f[] = {2, 4, 3, 1};
	double rms = Basic::rms(f, 0, 3);

	EXPECT_NEAR(2.73861, rms, EPS_TEST);
}

/**
 * Octave code:
 *
 * I = [2,4,3,1,6,3,8,2,3,6,1,7]
 * NBin =  4
 * P = hist (I(:), NBin, true)
 * P += (P == 0);
 * retval = -sum (P .* log2 (P));
 */
TEST(MATH_BASIC, ENTROPY) {

	double f[] = {2, 4, 3, 1, 6, 3, 9, 2, 3, 6, 1, 7};
	uint8_t binCnt = 4;
	double min = 1;
	double max = 9;

	double entropy = Basic::entropy(f, 0, 11, binCnt, min, max);
	EXPECT_NEAR(1.9183, entropy, EPS_TEST);

	double f2[] = {-0.2, 0.4, 0.3, 0.1, 0.6, 0.3, 0.9, 0.2, 0.3, 0.6, 0.1, 0.7};
	binCnt = 4;
	min = -0.2;
	max = 0.9;

	entropy = Basic::entropy(f2, 0, 11, binCnt, min, max);
	EXPECT_NEAR(1.7296, entropy, EPS_TEST);
}

