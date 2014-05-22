#include "../math/DTW.h"
#include "gtest/gtest.h"

using namespace wtm::math;

TEST(MATH_DTW, CALC_DISTANCE)
{
	uint32_t seq1size = 9;
	double* seq1 = new double[seq1size];
	seq1[0] = -2;
	seq1[1] = 10;
	seq1[2] = -10;
	seq1[3] = 15;
	seq1[4] = -13;
	seq1[5] = 20;
	seq1[6] = -5;
	seq1[7] = 14;
	seq1[8] = 2;

	uint32_t seq2size = 5;
	double* seq2 = new double[seq2size];
	seq1[0] = 3;
	seq1[1] = -13;
	seq1[2] = -7;
	seq1[3] = 9;
	seq1[4] = -2;

	double distance = DTW::calcDistance(seq1, seq1size, seq2, seq2size);
    EXPECT_EQ(34.2222, distance);
}



