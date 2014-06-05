#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/internal/gtest-internal.h>
#include <Processor.h>
#include <ModelCommand.h>
#include <Context.h>
#include "../audio/WavData.h"

using namespace yazz::audio;
using namespace yazz::command;

static string recognizeSample(string file) {

	Context* ctx = new Context();

	WavData* wavData = WavData::readFromFile(file);
	ctx->wavData = wavData;

	string theWord = ModelCommand::recognize(*ctx, NULL);
	delete ctx;

	return theWord;
}

static void testSetOfSamples(string folder) {

	EXPECT_EQ("odin",    recognizeSample(folder + "/1.wav"));
	EXPECT_EQ("dva",     recognizeSample(folder + "/2.wav"));
	EXPECT_EQ("tri",     recognizeSample(folder + "/3.wav"));
	EXPECT_EQ("chetire", recognizeSample(folder + "/4.wav"));
	EXPECT_EQ("pyat",    recognizeSample(folder + "/5.wav"));
	EXPECT_EQ("shest",   recognizeSample(folder + "/6.wav"));
	EXPECT_EQ("sem",     recognizeSample(folder + "/7.wav"));
	EXPECT_EQ("vosem",   recognizeSample(folder + "/8.wav"));
	EXPECT_EQ("devyat",  recognizeSample(folder + "/9.wav"));
	EXPECT_EQ("desyat",  recognizeSample(folder + "/10.wav"));
}

TEST(AUDIO_PROCESSOR, DISABLED_SPLITTING)
{
	testSetOfSamples("samples/male1");
	testSetOfSamples("samples/male2");
	testSetOfSamples("samples/male3");
	testSetOfSamples("samples/female1");
}





