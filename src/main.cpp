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

void draw(audio::WavDataPtr wavData);
void saveWords(audio::WavDataPtr wavData);

int main() {

	string sampleFile = "samples/male2.wav";
	audio::WavDataPtr wavData = audio::WavData::readFromFile(sampleFile);
	wavData->init();

	//draw(wavData);
	saveWords(wavData);

	cout << "Program completed successfully" << endl;
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

	cout << "Words: " << endl;
	for (vector<Word*>::const_iterator word = wavData->getWords().begin();
			word != wavData->getWords().end(); ++word) {

		cout << counter++ << ": " << (*word)->getFrames().size() << endl;

		//string fileName(sampleFile);
		//fileName + "." + toString(counter) + ".wav";
		//cout << fileName << endl;

		//wavData->saveToFile(fileName, *(*word));
	}
}
