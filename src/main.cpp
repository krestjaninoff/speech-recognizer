/**
 * An entry point for testing
 */
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include "audio/audio.h"
#include "audio/WavData.h"
#include "visual/PngDrawer.h"

using namespace std;
using namespace wtm;
using namespace wtm::audio;

int main() {

	string sampleFile = "samples/example.wav";
	audio::WavDataPtr wavData = audio::WavData::readFromFile(sampleFile);

	string diagramFile = "diagram.png";
	visual::PngDrawer::drawToFile(wavData, diagramFile);

	cout << "An diagram is successfully created!" << endl;
	return EXIT_SUCCESS;
}

