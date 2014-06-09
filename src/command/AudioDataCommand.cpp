#include <AudioDataCommand.h>
#include <Processor.h>
#include <WavData.h>
#include <Word.h>
#include <sys/stat.h>
#include <cstdio>
#include <cwchar>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "../common.h"

#ifdef __MINGW32__
#include <io.h>
#endif

using namespace std;
using namespace yazz::audio;

namespace yazz {
namespace command {

	const char* AudioDataCommand::OUTPUT_FOLDER_DEFAULT = "out";

	bool AudioDataCommand::readData(Context& context, const char* inputFile) {

		if (NULL == inputFile) {
			cout << "Input file is not specified" << endl;
			return false;
		}

		cout << "Reading WAV data..." << endl;
		audio::WavData* wavData = WavData::readFromFile(inputFile);

		if (NULL != wavData) {
			context.wavData = wavData;
		}

		return NULL != wavData;
	}

	bool AudioDataCommand::splitIntoFiles(Context& context, const char* outputFolder) {
		int counter = 1;
		cout << "Splitting data into separate words..." << endl;

		// Check pre-requirements
		if (NULL == context.wavData) {
			cout << "Input data is not specified :(" << endl;
			return false;
		}

		// Determine results directory name
		string folder(OUTPUT_FOLDER_DEFAULT);
		if (NULL != outputFolder) {
			string outputFolderStr(outputFolder);
			folder = outputFolderStr;
		}
		cout << "Output directory is " << folder << "..." << endl;

		// Check if output directory exists or can be created
		if (!initOutputDirectory(folder)) {
			return false;
		}

		// Create the Processor
		audio::Processor* processor = new Processor(context.wavData);
		processor->init();
		context.processor = processor;

		// Split wav data into words
		processor->divideIntoWords();

		// Save results
		DEBUG("Words: %d", processor->getWords()->size());
		for (vector<Word*>::const_iterator word = processor->getWords()->begin();
				word != processor->getWords()->end(); ++word) {

			string fileName = folder + "/" + toString(counter) + ".wav";
			cout << fileName << endl;

			processor->saveWordAsAudio(fileName, *(*word));
			counter++;
		}

		cout << "Complete!" << endl;
		return true;
	}

	bool AudioDataCommand::initOutputDirectory(const string& outputFolder) {
		struct stat info;

		if (0 != stat(outputFolder.c_str(), &info)) {
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

} /* namespace command */
} /* namespace yazz */



