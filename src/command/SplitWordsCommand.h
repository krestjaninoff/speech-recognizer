#ifndef SPLITWORDSCOMMAND_H_
#define SPLITWORDSCOMMAND_H_

#include <stdio.h>
#include <iostream>
#include <fstream>
//#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ICommand.h"
#include "../audio/Word.h"
#include "../audio/Processor.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace command {

/**
 * Split a sample into several words and store them into files
 */
class SplitWordsCommand : public ICommand {
public:
	SplitWordsCommand(const char* outputFolder) : outputFolder(outputFolder) {};

	bool execute(Context& context) {
		int counter = 1;
		cout << "Splitting data into separate words..." << endl;

		// Check pre-requirements
		if (NULL == context.wavData) {
			cout << "Input data is not specified :(" << endl;
			return false;
		}

		// Determine results directory name
		const char* outputFolderDefault = "out";
		string folder = outputFolderDefault;
		if (NULL != outputFolder) {
			string outputFolderStr(outputFolder);
			folder = outputFolderStr;
		}
		cout << "Output directory is " << folder << "..." << endl;

		// Check if output directory exists or can be created
		if (!checkOutput(outputFolder)) {
			return false;
		}

		// TODO Clean up the directory

		// Create the Processor
		audio::Processor* processor = new Processor(context.wavData);
		context.processor = processor;

		// Split wav data into words
		processor->split();

		// Save results
		DEBUG("Words: %d", processor->getWords()->size());
		for (vector<Word*>::const_iterator word = processor->getWords()->begin();
				word != processor->getWords()->end(); ++word) {

			string fileName = folder + "/sampleFile." + toString(counter) + ".wav";
			cout << fileName << endl;

			processor->saveWordAsAudio(fileName, *(*word));
			counter++;
		}

		cout << "Complete!" << endl;
		return true;
	};

private:
	const char* outputFolder;

	bool checkOutput(const string& outputFolder) {
		UNUSED(outputFolder);

		// FIXME Compilation error in io.h (MinGW)
		/*
		struct stat info;

		if (0 != stat( outputFolder.c_str(), &info )) {
			int success = mkdir(outputFolder.c_str());

			if (0 != success) {
				fprintf(stderr, "Directory %s can't be created\n", outputFolder.c_str());
				return false;
			} else {
				return true;
			}

		} else if (info.st_mode & S_IFDIR) {
			return true;

		} else {
			fprintf(stderr, "File %s is not a directory\n", outputFolder.c_str());
			return false;
		}
		*/

		return true;
	}
};

} /* namespace command */
} /* namespace wtm */

#endif /* SPLITWORDSCOMMAND_H_ */
