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
#include "../audio/Splitter.h"

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

		// Create the splitter
		audio::Splitter* splitter = new Splitter(context.wavData);
		context.splitter = splitter;

		// Split wav data into words
		splitter->split();

		// Save results
		DEBUG("Words: %d", splitter->getWords()->size());
		for (vector<Word*>::const_iterator word = splitter->getWords()->begin();
				word != splitter->getWords()->end(); ++word) {

			string fileName = folder + "/sampleFile." + toString(counter) + ".wav";
			cout << fileName << endl;

			splitter->saveWordAsAudio(fileName, *(*word));
			counter++;
		}

		cout << "Complete!" << endl;
		return true;
	};

private:
	const char* outputFolder;
	const string outputFolderDefault = "out";

	bool checkOutput(const string& outputFolder) {

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
