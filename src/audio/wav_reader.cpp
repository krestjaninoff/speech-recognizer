#include <iostream>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <wav_reader.h>

using namespace std;

namespace wtm {
namespace audio {

inline uint8_t measure16to8(int16_t value) {
	return (((long) value) + SHRT_MAX) * UCHAR_MAX / (2 * SHRT_MAX);
}

/**
 * Read Wav data from a file
 */
TWavDataPtr WavReader::readFromFile(const std::string& file) {
	TWavHeader wavHeader;
	TWavDataPtr wavData(new TWavData());

	// Open file
	FILE *wavFile = fopen(file.c_str() , "rb");
	if (wavFile == NULL) {
		fprintf(stderr, "Can not able to open wave file\n");
		exit(EXIT_FAILURE);
	}

	// Read header
	fread(&wavHeader, sizeof(TWavHeader), 1, wavFile);
	checkData(wavHeader);

	// Read raw data
	uint8_t value, valueLeft, valueRight, minValue = 0, maxValue = 0;
	int16_t value16, valueLeft16, valueRight16;
	long value16measured;
	unsigned long i = 0;
	unsigned long numberOfSamples = wavHeader.subchunk2Size / (wavHeader.numOfChan * wavHeader.bitsPerSample / 8);
	unsigned long bytesPerChannel = wavHeader.blockAlign / wavHeader.numOfChan;
	for (; i < numberOfSamples && !feof(wavFile); i++) {

		if (1 == numberOfSamples) {
			if (1 == wavHeader.numOfChan) {
				fread(&value, sizeof(uint8_t), 1, wavFile);

			} else {
				fread(&valueLeft, sizeof(raw_t), 1, wavFile);
				fread(&valueRight, sizeof(raw_t), 1, wavFile);
				value = (valueLeft + valueRight) / 2;
			}
		} else {
			if (1 == wavHeader.numOfChan) {
				fread(&value16, sizeof(int16_t), 1, wavFile);
				value = measure16to8(value16);

			} else {
				fread(&valueLeft16, sizeof(int16_t), 1, wavFile);
				fread(&valueRight16, sizeof(int16_t), 1, wavFile);
				value = (measure16to8(valueLeft16) + measure16to8(valueRight16)) / 2;
			}
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
 */
void WavReader::checkData(const TWavHeader& wavHeader) {

	if (0 != strncmp(wavHeader.riff, "RIFF", sizeof(wavHeader.riff))
			|| 0 != strncmp(wavHeader.wave, "WAVE", sizeof(wavHeader.wave))) {
		fprintf(stderr, "Invalid RIFF/WAVE format\n");
		exit(EXIT_FAILURE);
	}

	if (1 != wavHeader.audioFormat) {
		fprintf(stderr, "Invalid WAV format: only PCM audio format is supported\n");
		exit(EXIT_FAILURE);
	}

	if (wavHeader.numOfChan > 2) {
		fprintf(stderr, "Invalid WAV format: only 1 or 2 channels audio is supported\n");
		exit(EXIT_FAILURE);
	}

	unsigned long bitsPerChannel = wavHeader.bitsPerSample / wavHeader.numOfChan;
	if (8 != bitsPerChannel && 16 != bitsPerChannel) {
		fprintf(stderr, "Invalid WAV format: only 8 and 16-bit per channel is supported\n");
		exit(EXIT_FAILURE);
	}

	if (wavHeader.subchunk2Size > LONG_MAX) {
		fprintf(stderr, "File too big\n");
		exit(EXIT_FAILURE);
	}
}

} // namespace audio
} // namespace wtm
