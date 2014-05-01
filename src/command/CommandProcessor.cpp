#include "CommandProcessor.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>

namespace wtm {
namespace command {

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
				abort();
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
