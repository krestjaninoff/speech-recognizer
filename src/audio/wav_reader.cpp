#include <iostream>
#include <fstream>
#include <stdio.h>

#include <wav_reader.h>

using namespace std;

namespace wtm {
namespace audio {

/**
 * Read Wav data from a file
 */
wav_data_t* readWav(std::string file) {
	wav_hdr_t wav_hdr;
	wav_data_t* wav_data = new wav_data_t();

	FILE *wavFile = fopen(file.c_str() , "r");
	if (wavFile == NULL) {
		fprintf(stderr, "Can not able to open wave file\n");
		exit(EXIT_FAILURE);
	}

	fread(&wav_hdr, sizeof(wav_hdr_t), 1, wavFile);
	if (0 != strcmp(wav_hdr.RIFF, "RIFF")) {
		fprintf(stderr, "Invalid WAV format\n");
		exit(EXIT_FAILURE);
	}

	int16_t value;
	unsigned long i=0;
	for (; i < wav_hdr.Subchunk2Size; i++) {
		fread(&value, sizeof(int16_t), 1, wavFile);

		if (wav_data->max_x < value) {
			wav_data->max_x = value;
		}

		if (wav_data->min_x > value) {
			wav_data->min_x = value;
		}

		wav_data->data->push_back(value);
	}
	wav_data->duration = i;

	fclose(wavFile);

	return wav_data;
}

} // namespace audio
} // namespace wtm
