/**
 * Entry point for testing
 */
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include "common.h"
#include "audio/audio.h"
#include "audio/WavData.h"
#include "visual/PngDrawer.h"

#define toString( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
using namespace wtm;
using namespace wtm::audio;

void draw(audio::WavDataPtr wavData);
void saveWords(audio::WavDataPtr wavData);

int main() {

	string sampleFile = "samples/female1.wav";
	audio::WavDataPtr wavData = audio::WavData::readFromFile(sampleFile);
	wavData->init();

	//draw(wavData);
	saveWords(wavData);

	DEBUG("!!!\nProgram completed successfully\n!!!");
	return EXIT_SUCCESS;
}

void draw(audio::WavDataPtr wavData) {
	//string diagramRaw = "raw.png";
	//visual::PngDrawer::drawRawData(wavData, diagramRaw);

	string diagramFrame = "frame.png";
	visual::PngDrawer::drawFrames(wavData, diagramFrame);
}

void saveWords(audio::WavDataPtr wavData) {
	int counter = 1;

	DEBUG("Words: %d", wavData->getWords().size());
	for (vector<Word*>::const_iterator word = wavData->getWords().begin();
			word != wavData->getWords().end(); ++word) {

		string fileName = "sampleFile." + toString(counter) + ".wav";
		cout << fileName << endl;

		wavData->saveToFile(fileName, *(*word));
		counter++;
	}
}
