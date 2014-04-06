#include <iostream>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <wav_reader.h>

using namespace std;

namespace wtm {
namespace audio {

/**
 * Read Wav data from a file
 */
TWavDataPtr WavReader::readFromFile(const std::string& file) {
	TWavHeader wavHeader;
	TWavDataPtr wavData(new TWavData());

	// Open file
	FILE *wavFile = fopen(file.c_str() , "r");
	if (wavFile == NULL) {
		fprintf(stderr, "Can not able to open wave file\n");
		exit(EXIT_FAILURE);
	}

	// Read header
	fread(&wavHeader, sizeof(TWavHeader), 1, wavFile);
	checkData(wavHeader);

	// Read raw data
	raw_t value, valueRight, valueLeft, minValue = 0, maxValue = 0;
	uint32_t i = 0;
	for (; i < wavHeader.subchunk2Size; i++) {

		if (sizeof(raw_t) * 8 == wavHeader.bitsPerSample) {
			fread(&value, sizeof(raw_t), 1, wavFile);

		} else if (sizeof(raw_t) * 8  * 2 == wavHeader.bitsPerSample) {
			fread(&valueLeft, sizeof(raw_t), 1, wavFile);
			fread(&valueRight, sizeof(raw_t), 1, wavFile);
			value = (valueLeft + valueRight) / 2;
		}

		if (maxValue < value) {
			maxValue = value;
		}

		if (minValue > value) {
			minValue = value;
		}

		wavData->getRawData()->push_back(value);
	}

	wavData->setMinVal(minValue);
	wavData->setMaxVal(maxValue);
	wavData->setDuration(i);

	fclose(wavFile);

	return wavData;
}

/**
 * Checks a set of restrictions
 *
 * Ideally, all these restrictions should be removed.
 */
void WavReader::checkData(const TWavHeader& wavHeader) {

	if (0 != strncmp(wavHeader.riff, "RIFF", sizeof(wavHeader.riff))
			|| 0 != strncmp(wavHeader.wave, "WAVE", sizeof(wavHeader.riff))) {
		fprintf(stderr, "Invalid WAV format: %s / %s\n", wavHeader.riff, wavHeader.wave);
		exit(EXIT_FAILURE);
	}

	// Let's think that all these formats just compress 16-bit audio down to 8-bit
	// http://www.threejacks.com/?q=node/176
	/*
	if (1 != wavHeader.audioFormat) {
		fprintf(stderr, "Invalid WAV format: only PCM audio format is supported\n");
		exit(EXIT_FAILURE);
	}
	*/

	if (sizeof(raw_t) * 8 != wavHeader.bitsPerSample) {
		fprintf(stderr, "Invalid WAV format: only %d-bit audio is supported\n",
				sizeof(raw_t) * 8);
		exit(EXIT_FAILURE);
	}

	if (wavHeader.numOfChan > 2) {
		fprintf(stderr, "Invalid WAV format: only 1 or 2 channels audio is supported\n");
		exit(EXIT_FAILURE);
	}
}

} // namespace audio
} // namespace wtm
