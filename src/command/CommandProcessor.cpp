#include "CommandProcessor.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include "../common.h"

namespace wtm {
namespace command {

	/**
	 * Commands map
	 */
	static struct option longOptions[] = {
		{ "version", no_argument, 0, 'v' },
		{ "help", no_argument, 0, 'h' },
		{ "input", required_argument, 0, 'i' },
		{ "draw", optional_argument, 0, 'd' },
		{ "split", optional_argument, 0, 's' },
		{ "add-model", optional_argument, 0, 'a' },
		{0, 0, 0, 0}
	};
	static const char* const shortOptions = "vhi:d::s::a::";

	/**
	 * Help info
	 */
	static const char* const helpInfo =	EOL
		"Usage: wtm [options]" EOL
		EOL
		"Options:" EOL
		"-v, --version \t\t Display version information" EOL
		"-h, --help \t\t Display help information (this manual)" EOL
		"-i, --input \t\t Read WAV data for post-processing" EOL
		"-d<filename>, --draw=<filename> \t Create the RMS diagram based on the WAV data (requires -i) and store into the specified file (file name is optional)" EOL
		"-s<filename>, --split=<dirname> \t Split the WAV data (requires -i) into words and store them into the specified directory (file name is optional)" EOL
		EOL
		"Notice, because of GetOpt restriction you must set optional parameters in the same way which you see in this manual."
		EOL;

	bool CommandProcessor::process() {

		if (argc > 1) {

			bool isLast = false;
			while (!isLast) {
				int optionIndex = 0;
				int c = getopt_long(argc, argv, shortOptions, longOptions, &optionIndex);

				/* Detect the end of the options. */
				if (c == -1)
					break;

				switch (c) {

					case 'v':
						printVersion();

						isLast = true;
						break;

					case 'h':
						printVersion();
						printHelp();

						isLast = true;
						break;

					case 'i':
						isLast = !(new AudioDataCommand())->readData(*this->context, optarg);
						break;

					case 'd':
						isLast = !(new VisualizationCommand(optarg))->createDiagram(*this->context);
						break;

					case 's':
						isLast = !(new AudioDataCommand())->splitIntoFiles(*this->context, optarg);
						break;

					case 'a':
						isLast = !(new ModelCommand())->addModel(*this->context);
						break;

					default:
						cout << "Invalid parameter: " << (char) c << endl;
						cout << "Please, use -h (--help) for details." << (char) c << endl;
						return false;
				}
			}

		} else {
			cout << "No input parameters specified. Please take a look on the help info for details: " << endl;
			printHelp();
		}

		return true;
	}

	void CommandProcessor::printVersion() {
		cout << PROJECT_NAME << " version " << PROJECT_VERSION << endl;
	}

	void CommandProcessor::printHelp() {
		cout << helpInfo << endl;
	}

} /* namespace cpmmand */
} /* namespace wtm */
