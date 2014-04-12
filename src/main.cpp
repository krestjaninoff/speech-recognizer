/**
 * An entry point for testing
 */
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include "audio/audio.h"
#include "audio/WavData.h"
#include "visual/PngDrawer.h"
#include <sstream>

#define toString( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
using namespace wtm;
using namespace wtm::audio;

int main() {

	string sampleFile = "samples/male1.wav";
	audio::WavDataPtr wavData = audio::WavData::readFromFile(sampleFile);
	wavData->init();

	//string diagramRaw = "raw.png";
	//visual::PngDrawer::drawRawData(wavData, diagramRaw);

	//string diagramFrame = "frame.png";
	//visual::PngDrawer::drawFrames(wavData, diagramFrame);

	int i = 1;
	for (vector<Word*>::const_iterator word = wavData->getWords().begin();
			word != wavData->getWords().end(); ++word) {

		cout << i++ << endl;

		/*
		string fileName(sampleFile);
		fileName + "." + toString(i) + ".wav";
		cout << fileName << endl;

		wavData->saveToFile(fileName, *(*word));
		*/
	}

	cout << "An diagram is successfully created!" << endl;
	return EXIT_SUCCESS;
}

