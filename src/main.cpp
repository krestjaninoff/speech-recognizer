/**
 * An entry point for testing
 */
#include <iostream>
#include <stdlib.h>
#include "audio/wav_reader.h"
#include "visual/png_drawer.h"

using namespace std;
using namespace wtm;

int main(int argc, char **argv) {

	char* sampleFile = "samples/example.wav";
	audio::wav_data_t* wav_data = audio::readWav(sampleFile);

	for (list<int16_t>::iterator it=wav_data->data->begin(); it != wav_data->data->end(); ++it) {
	    cout << it << endl;
	}

	char* diagramFile = "diagram.png";
	visual::draw_png(wav_data, diagramFile);

	return EXIT_SUCCESS;
}

