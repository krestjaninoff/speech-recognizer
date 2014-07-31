#include "../config.h"
#include <AudioDataCommand.h>
#include <CommandProcessor.h>
#include <ModelCommand.h>
#include <VisualizationCommand.h>
#include <getopt.h>
#include <iostream>

namespace yazz {
namespace command {

/**
 * Commands map
 */
static struct option longOptions[] = {
	{ "version", no_argument, 0, 'v' },
	{ "help", no_argument, 0, 'h' },
	{ "input", required_argument, 0, 'i' },

	{ "list-models", optional_argument, 0, 'l' },
	{ "list-observations", optional_argument, 0, 'L' },

	{ "print-model", optional_argument, 0, 'p' },
	{ "print-observation", optional_argument, 0, 'P' },

	{ "add-model", optional_argument, 0, 'a' },
	{ "add-observation", optional_argument, 0, 'A' },

	{ "delete-model", optional_argument, 0, 'd' },
	{ "delete-observation", optional_argument, 0, 'D' },

	{ "train-model", optional_argument, 0, 't' },
	{ "recognize", optional_argument, 0, 'r' },

	{ "graph", optional_argument, 0, 'g' },
	{ "split", optional_argument, 0, 's' },

	{0, 0, 0, 0}
};
static const char* const shortOptions = "vhi:lLp:P:a:A:d:D:t:r:g::s::";

/**
 * Help info
 */
static const char* const helpInfo =	EOL
	"Usage: yazz [options]" EOL
	EOL

	"Options:" EOL
	"-v, --version \t\t Display version information" EOL
	"-h, --help    \t\t Display help information (this manual)" EOL
	"-i, --input   \t\t Read WAV data for post-processing" EOL

	EOL

	"-l,    --list-models				\t\t Show the list of the available models" EOL
	"-L,    --list-observations			\t\t Show the list of the available observations (CodeBook)" EOL
	"-p<m>, --print-model=<m>			\t\t Print the specific model by given Id" EOL
	"-P<o>, --print-observation=<o>		\t\t Print the specific observation by given Label" EOL

	EOL

	"-a<m>, --add-model=<m>				\t\t Add the model with given name" EOL
	"-A<o>, --add-observation=<o>		\t\t Add the observation with given label" EOL
	"-d<m>, --delete-model=<m>			\t\t Delete the model with given Id" EOL
	"-D<o>, --delete-observation=<o>	\t\t Delete the observation with given label" EOL

	EOL

	"-t<m>, --train-model<m>			\t\t Train the specific model with the input data" EOL
	"-r<m1,m2>, --recognize=<m1,m2>		\t\t Recognize the input data using specified (csv) list of models (by default all models are used)" EOL

	EOL

	"-g<filename>, --graph=<filename>	\t Create the RMS diagram based on the WAV data (requires -i) and store into the specified file (file name is optional)" EOL
	"-s<filename>, --split=<dirname>	\t Split the WAV data (requires -i) into words and store them into the specified directory (file name is optional)" EOL
	EOL

	"Notice, because of GetOpt restriction you must set optional parameters in the same way which you see in this manual."
	EOL;

bool CommandProcessor::process() {

	if (argc > 1) {

		bool isLast;
		do {
			isLast = true;
			int optionIndex = 0;
			int c = getopt_long(argc, argv, shortOptions, longOptions, &optionIndex);

			/* Detect the end of the options. */
			if (c == -1)
				break;

			switch (c) {

				case 'v':
					printVersion();
					break;
				case 'h':
					printVersion();
					printHelp();
					break;

				case 'i':
					isLast = !AudioDataCommand::readData(*this->context, optarg);
					isLast = false;
					break;

				case 'l':
					ModelCommand::listModels(*this->context);
					break;
				case 'L':
					ModelCommand::listObservations(*this->context);
					break;

				case 'p':
					ModelCommand::printModel(*this->context, optarg);
					break;
				case 'P':
					ModelCommand::printObservation(*this->context, optarg);
					break;

				case 'a':
					ModelCommand::addModel(*this->context, optarg);
					break;
				case 'A':
					ModelCommand::addObservation(*this->context, optarg);
					break;
				case 'd':
					ModelCommand::deleteModel(*this->context, optarg);
					break;
				case 'D':
					ModelCommand::deleteObservation(*this->context, optarg);
					break;

				case 't':
					ModelCommand::trainModel(*this->context, optarg);
					break;
				case 'r':
					ModelCommand::recognize(*this->context, optarg);
					break;

				case 'g':
					isLast = !VisualizationCommand::createDiagram(*this->context, optarg);
					break;
				case 's':
					isLast = !AudioDataCommand::splitIntoFiles(*this->context, optarg);
					break;

				default:
					cout << "Invalid parameter: " << (char) c << endl;
					cout << "Please, use -h (--help) for details." << (char) c << endl;
					return false;
			}
		} while (!isLast);

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

} /* namespace command */
} /* namespace yazz */
