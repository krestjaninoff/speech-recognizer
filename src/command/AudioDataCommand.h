#ifndef AUDIODATACOMMAND_H_
#define AUDIODATACOMMAND_H_

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Context.h"
#include "../audio/Word.h"
#include "../audio/Processor.h"

using namespace std;
using namespace yazz::audio;

namespace yazz {
namespace command {

/**
 * Split a sample into several words and store them into files
 */
class AudioDataCommand {
public:

	bool readData(Context& context, const char* inputFile) {
		cout << "Reading WAV data..." << endl;

		audio::WavData* wavData = audio::WavData::readFromFile(inputFile);

		if (NULL != wavData) {
			context.wavData = wavData;
		}

		return NULL != wavData;
	};

	bool splitIntoFiles(Context& context, const char* outputFolder) {
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
		if (!initOutputDirectory(outputFolder)) {
			return false;
		}

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

	bool initOutputDirectory(const string& outputFolder) {
		struct stat info;

		if (0 != stat( outputFolder.c_str(), &info )) {
			int success;

			#if defined(_WIN32)
				success = _mkdir(outputFolder.c_str());
			#else
				success = mkdir(outputFolder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
			#endif

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

		return true;
	}
};

} /* namespace command */
} /* namespace yazz */

#endif /* AUDIODATACOMMAND_H_ */
