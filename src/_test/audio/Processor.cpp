#include "../audio/WavData.h"
#include "../audio/Processor.h"
#include "gtest/gtest.h"

using namespace yazz::audio;

static int splitSample(const char* file) {
	WavData* wavData = WavData::readFromFile(file);
	Processor* processor = new Processor(wavData);
	processor->split();

	int wordsCount = processor->getWords()->size();

	delete processor;
	delete wavData;

	return wordsCount;
}

TEST(AUDIO_PROCESSOR, SPLITTING)
{
	int wordsCount;

	wordsCount = splitSample("samples/male1.wav");
	EXPECT_EQ(10, wordsCount);

	wordsCount = splitSample("samples/male2.wav");
	EXPECT_EQ(10, wordsCount);

	wordsCount = splitSample("samples/male3.wav");
	EXPECT_EQ(10, wordsCount);

	wordsCount = splitSample("samples/female1.wav");
	EXPECT_EQ(10, wordsCount);
}





