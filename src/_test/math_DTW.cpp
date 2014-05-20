#include "../math/DTW.h"
#include "gtest/gtest.h"

using namespace wtm::math;

TEST(MATH_DTW, CALC_DISTANCE)
{
	uint32_t seq1size = 3;
	double* seq1 = new double[seq1size];
	seq1[0] = 1;
	seq1[1] = 4;
	seq1[2] = 2;

	uint32_t seq2size = 4;
	double* seq2 = new double[seq2size];
	seq1[0] = 2;
	seq1[1] = 1;
	seq1[2] = 4;
	seq1[3] = 3;

	// TODO FixMe
	double distance = DTW::calcDistance(seq1, seq1size, seq2, seq2size);

    EXPECT_EQ(5, distance);
}



