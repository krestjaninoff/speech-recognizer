/*
 * ReadWavDataCommand.h
 *
 *  Created on: 13 May 2014
 *      Author: kresmik
 */

#ifndef READWAVDATACOMMAND_H_
#define READWAVDATACOMMAND_H_

#include "../audio/WavData.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace command {

/**
 * Read wav data from a file
 */
class ReadWavDataCommand : public ICommand {
public:
	ReadWavDataCommand(const char* inputFile) : inputFile(inputFile) {};

	bool execute(Context& context) {
		cout << "Reading WAV data..." << endl;

		audio::WavData* wavData = audio::WavData::readFromFile(inputFile);

		if (NULL != wavData) {
			context.wavData = wavData;
		}

		return NULL != wavData;
	};

private:
	const char* inputFile;
};

} /* namespace command */
} /* namespace wtm */

#endif /* READWAVDATACOMMAND_H_ */
