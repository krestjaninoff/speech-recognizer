#include <iostream>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <WavData.h>

using namespace std;

namespace wtm {
namespace audio {

/**
 * Read Wav data from a file
 */
WavDataPtr WavData::readFromFile(const std::string& file) {
	WavHeader wavHeader;
	WavDataPtr wavFile(new WavData());

	// Open file
	std::fstream fs;
	fs.open(file.c_str(), std::ios::in | std::ios::binary);

	if (!fs.good()) {
		fprintf(stderr, "Can't open the wave file\n");
		exit(EXIT_FAILURE);
	}

	// Read header
	fs.read((char*)(&wavHeader), sizeof(WavHeader));
	checkHeader(wavHeader);

	// Read raw data
	readRawData(fs, wavHeader, *wavFile);
	fs.close();

	return wavFile;
}

/**
 * Checks a set of restrictions
 */
void WavData::checkHeader(const WavHeader& wavHeader) {

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

void WavData::readRawData(std::fstream& fs, const WavHeader& wavHeader, WavData& wavFile) {
	raw_t value, minValue = 0, maxValue = 0;
	uint8_t value8, valueLeft8, valueRight8;
	int16_t value16, valueLeft16, valueRight16;

	unsigned long sampleNumber = 0;
	unsigned long numberOfSamples = wavHeader.subchunk2Size / (wavHeader.numOfChan * wavHeader.bitsPerSample / 8);
	unsigned long bytesPerChannel = wavHeader.blockAlign / wavHeader.numOfChan;

	for (; sampleNumber < numberOfSamples && !fs.eof(); sampleNumber++) {

		if (8 == wavHeader.bitsPerSample) {
			if (1 == wavHeader.numOfChan) {
				fs.read((char*)(&value8), sizeof(uint8_t));
				value = static_cast<raw_t>(value8);

			} else {
				fs.read((char*)(&valueLeft8), sizeof(uint8_t));
				fs.read((char*)(&valueRight8), sizeof(uint8_t));
				value = static_cast<raw_t>((valueLeft8 + valueRight8) / 2);
			}
		} else {
			if (1 == wavHeader.numOfChan) {
				fs.read((char*)(&value16), sizeof(int16_t));
				value = static_cast<raw_t>(value16);

			} else {
				fs.read((char*)(&valueLeft16), sizeof(int16_t));
				fs.read((char*)(&valueRight16), sizeof(int16_t));
				value = static_cast<raw_t>((valueLeft16 + valueRight16) / 2);
			}
		}

		if (maxValue < value) {
			maxValue = value;
		}

		if (minValue > value) {
			minValue = value;
		}
		wavFile.getRawData().push_back(value);
	}

	// Update values
	wavFile.setMinVal(minValue);
	wavFile.setMaxVal(maxValue);
	wavFile.setNumberOfSamples(sampleNumber);
}

} // namespace audio
} // namespace wtm
