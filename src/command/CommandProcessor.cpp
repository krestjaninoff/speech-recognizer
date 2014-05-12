#include "CommandProcessor.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include "../audio/WavData.h"

namespace wtm {
namespace command {

	/**
	 * Commands map
	 */
	static struct option longOptions[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "input", required_argument, NULL, 'i' },
		{ "draw", optional_argument, NULL, 'd' },
		{ "split", optional_argument, NULL, 's' },
		{0, 0, 0, 0}
	};
	static const char* const optionsStop = "h:id:is:";

	bool CommandProcessor::process() {

		int c;
		while (1) {

			int optionIndex = 0;
			c = getopt_long(argc, argv, optionsStop, longOptions, &optionIndex);

			/* Detect the end of the options. */
			if (c == -1)
				break;

			switch (c) {

			case 'h':
				(new HelpCommand())->execute(*this->context);
				break;

			case 'i':
				this->readWavData(optarg);
				break;

			case 'd':
				(new DrawDiagramCommand(optarg))->execute(*this->context);
				break;

			case 's':
				(new SplitWordsCommand(optarg))->execute(*this->context);
				break;

			default:
				cout << "Invalid parameter: " << (char) c << endl;
				return false;
			}
		}

		return true;
	}

	void CommandProcessor::readWavData(const string& source) {
		//string sampleFile = "samples/female1.wav";

		audio::WavData* wavData = audio::WavData::readFromFile(source);
		this->context->wavData = wavData;
	}

} /* namespace cpmmand */
} /* namespace wtm */
