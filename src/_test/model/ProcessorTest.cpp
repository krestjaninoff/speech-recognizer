#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include "../config.h"
#include "../model/Processor.h"

using namespace yazz;

TEST(CODEBOOK, FIND_OBSERVATION) {

	Storage* storage = new Storage();
	storage->init();
	Processor* processor = new Processor(storage);

	vector<MfccEntry*>* mfcc = new vector<MfccEntry*>();

	// O
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {89.9135, 9.86522, 17.0171, -2.03679, -6.10831, -0.290932, -6.37831, 4.51551, -0.713989, 0.721692, -0.0719165, -2.16403}));
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {86.5157, 9.17449, 18.5488, -3.63196, -5.21477, -0.684599, -5.00631, 3.08095, -1.39163, -0.503935, -0.762257, -2.50522}));
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {66.1171, 11.2775, 20.3188, -5.77797, -7.55167, 1.49457, -5.48048, 2.72151, 0.0930632, -1.11133, -1.33383, -2.86237}));

	// D
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {72.1449, -0.635406, 6.08631, 5.78902, 7.255, 1.36787, 1.22121, 1.15373, -0.705021, 1.04118, -2.89717, 0.394255}));
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {46.5031, 1.58009, 4.49197, 3.55106, 8.55179, 3.22173, 1.81647, 2.57726, 1.40443, 2.84506, -1.55286, 0.248423}));

	// I
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {57.9754, -15.68, 16.8849, 4.2688, 6.36357, 0.917356, 6.04843, 3.97115, -0.348592, 3.67503, -0.649177, 0.242363}));
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {54.1651, -13.8821, 16.4634, 5.27518, 6.12481, 0.522569, 5.52317, 2.54984, -0.473555, 3.36687, -0.824478, 0.64014}));
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {48.9572, -14.7293, 15.3592, 5.15431, 5.3934, -0.479035, 4.17092, 0.724135, -1.98377, 2.66236, -0.666084, 0.356335}));

	// N
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {75.5338, 1.82504, 8.71286, 0.921262, -3.04727, 6.74151, 3.36035, -1.20071, -0.91626, -0.956197, 1.59809, -0.112569}));
	mfcc->push_back(new MfccEntry(new double[MFCC_SIZE] {57.3562, 5.26148, 8.94081, 0.232647, -1.92958, 6.30622, 3.27968, -2.17749, -1.04542, -1.54835, 1.59205, -0.00333686}));

	const vector<observation_t>* result = processor->mfccToObservations(mfcc);

	ASSERT_STREQ("o", result->at(0).c_str());
	ASSERT_STREQ("o", result->at(1).c_str());
	ASSERT_STREQ("o", result->at(2).c_str());

	ASSERT_STREQ("d", result->at(3).c_str());
	ASSERT_STREQ("d", result->at(4).c_str());

	ASSERT_STREQ("i", result->at(5).c_str());
	ASSERT_STREQ("i", result->at(6).c_str());
	ASSERT_STREQ("i", result->at(7).c_str());

	ASSERT_STREQ("n", result->at(8).c_str());
	ASSERT_STREQ("n", result->at(9).c_str());

	delete processor;
	delete result;
}
