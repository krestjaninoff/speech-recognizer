/**
 * An entry point for testing
 */
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include "audio/audio.h"
#include "audio/wav_reader.h"
#include "visual/png_drawer.h"

using namespace std;
using namespace wtm;
using namespace wtm::audio;

int main() {

	string sampleFile = "samples/example.wav";
	audio::TWavDataPtr wavData = audio::WavReader::readFromFile(sampleFile);

	/*for (list<raw_t>::iterator yCurr = wavData->getRawData()->begin();
				yCurr != wavData->getRawData()->end(); ++yCurr) {
		cout << (int) *yCurr << endl;
	}*/

	string diagramFile = "diagram.png";
	visual::PngDrawer::drawToFile(wavData, diagramFile);
	cout << "An diagram is successfully created!" << endl;

	return EXIT_SUCCESS;
}

