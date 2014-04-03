/**
 * An entry point for testing
 */
#include <iostream>
#include "audio/wav_reader.h"
#include "visual/png_drawer.h"

using namespace std;
using namespace wtm;

int main(int argc, char **argv) {

	char* sampleFile = "samples/example.wav";
	wav_data_t* data = audio::readWav(sampleFile);

	for (wav_data_t::iterator it=data->begin(); it != data->end(); ++it) {
	    cout << it << endl;
	}

	char* diagramFile = "diagram.png";
	visual::draw_png(data, diagramFile);

	return EXIT_SUCCESS;
}

