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
	wav_data_t* data = new wav_data_t;

	FILE *wavFile = fopen(file , "r");
	if(wavFile == NULL){
		fprintf(stderr, "Can not able to open wave file\n");
		exit(EXIT_FAILURE);
	}

	fread(&wav_hdr, sizeof(wav_hdr_t), 1, wavFile);

	int16_t value;
	for (int i=0; i < wav_hdr.Subchunk2Size; i++) {
		fread(&value, sizeof(int16_t), 1, wavFile);
		data->add(value);
	}

	fclose(wavFile);

	return data;
}

} // namespace audio
} // namespace wtm
