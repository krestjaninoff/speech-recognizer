#include "gtest/gtest.h"
#include "../math/DTW.h"

TEST(MATH_DTW, BASIC_TEST)
{
	int seq1size = 3;
	double* seq1 = new double[seq1size];
	seq1[0] = 1;
	seq1[1] = 4;
	seq1[2] = 2;

	int seq2size = 4;
	double* seq2 = new double[seq2size];
	seq1[0] = 2;
	seq1[1] = 1;
	seq1[2] = 4;
	seq1[3] = 3;

	double distance = DTW::calcDistance(seq1, seq1size, seq2, seq2size);

    EXPECT_EQ(5, distance);
}



