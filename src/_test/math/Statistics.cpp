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

	EXPECT_TRUE(fabs(rms - 2.7386) < numeric_limits<double>::epsilon());
}

TEST(MATH_STATISTICS, ENTROPY)
{
	vector<raw_t> data;
	data.push_back(2);
	data.push_back(4);
	data.push_back(3);
	data.push_back(1);
	data.push_back(6);
	data.push_back(3);
	data.push_back(8);
	data.push_back(2);
	data.push_back(3);
	data.push_back(6);
	data.push_back(1);
	data.push_back(7);

	const vector<raw_t>& dataRef = data;
	double rms = Statistics::entropy(dataRef, 0, data.size() - 1, 4, 0, 10);

	EXPECT_TRUE(fabs(rms - 2.7386) < numeric_limits<double>::epsilon());
}

