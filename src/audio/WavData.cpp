#include <iostream>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <limits>
#include <limits.h>
#include <cassert>
#include <vector>
#include <math.h>
#include "audio.h"
#include "WavData.h"
#include "Frame.h"
#include "Word.h"

using namespace std;

namespace wtm {
namespace audio {

/**
 * Read Wav data from a file
 */
WavData* WavData::readFromFile(const std::string& file) {
	WavHeader wavHeader;

	// Open file
	std::fstream fs;
	fs.open(file.c_str(), std::ios::in | std::ios::binary);

	if (!fs.good()) {
		fprintf(stderr, "Input file not found\n");
		return NULL;
	}

	// Read header
	fs.read((char*)(&wavHeader), sizeof(WavHeader));
	if (!checkHeader(wavHeader)) {
		return NULL;
	}

	// Read raw data
	WavData* wavData = new WavData(wavHeader);
	readRawData(fs, wavHeader, *wavData);
	fs.close();

	return wavData;
}

/**
 * Checks a set of restrictions
 */
bool WavData::checkHeader(const WavHeader& wavHeader) {

	if (0 != strncmp(wavHeader.riff, "RIFF", sizeof(wavHeader.riff))
			|| 0 != strncmp(wavHeader.wave, "WAVE", sizeof(wavHeader.wave))) {
		fprintf(stderr, "Invalid RIFF/WAVE format\n");
		return false;
	}

	if (1 != wavHeader.audioFormat) {
		fprintf(stderr, "Invalid WAV format: only PCM audio format is supported\n");
		return false;
	}

	if (wavHeader.numOfChan > 2) {
		fprintf(stderr, "Invalid WAV format: only 1 or 2 channels audio is supported\n");
		return false;
	}

	unsigned long bitsPerChannel = wavHeader.bitsPerSample / wavHeader.numOfChan;
	if (8 != bitsPerChannel && 16 != bitsPerChannel) {
		fprintf(stderr, "Invalid WAV format: only 8 and 16-bit per channel is supported\n");
		return false;
	}

	assert(wavHeader.subchunk2Size > 0);
	if (wavHeader.subchunk2Size > LONG_MAX) {
		fprintf(stderr, "File too big\n");
		return false;
	}

	return true;
}

void WavData::readRawData(std::fstream& fs, const WavHeader& wavHeader, WavData& wavFile) {
	raw_t value, minValue = 0, maxValue = 0;
	uint8_t value8, valueLeft8, valueRight8;
	int16_t value16, valueLeft16, valueRight16;

	length_t bytesPerSample = static_cast<uint32_t>(wavHeader.bitsPerSample / 8);
	unsigned long numberOfSamplesXChannels = wavHeader.subchunk2Size /
			(wavHeader.numOfChan * bytesPerSample);

	unsigned long sampleNumber = 0;
	for (; sampleNumber < numberOfSamplesXChannels && !fs.eof(); sampleNumber++) {

		// TODO Should we match 8 bit values to 16 bit? Vice versa?
		if (8 == wavHeader.bitsPerSample) {
			if (1 == wavHeader.numOfChan) {
				fs.read((char*)(&value8), sizeof(uint8_t));
				value = static_cast<raw_t>(value8);

			} else {
				fs.read((char*)(&valueLeft8), sizeof(uint8_t));
				fs.read((char*)(&valueRight8), sizeof(uint8_t));
				value = static_cast<raw_t>((abs(valueLeft8) + abs(valueRight8)) / 2);
			}
		} else {
			if (1 == wavHeader.numOfChan) {
				fs.read((char*)(&value16), sizeof(int16_t));
				value = static_cast<raw_t>(value16);

			} else {
				fs.read((char*)(&valueLeft16), sizeof(int16_t));
				fs.read((char*)(&valueRight16), sizeof(int16_t));
				value = static_cast<raw_t>((abs(valueLeft16) + abs(valueRight16)) / 2);
			}
		}

		if (maxValue < value) {
			maxValue = value;
		}

		if (minValue > value) {
			minValue = value;
		}

		//wavFile.rawData->push_back(value);
		wavFile.rawData->insert(wavFile.rawData->begin() + sampleNumber, value);
	}
	assert(sampleNumber > 0);

	// Update values
	wavFile.setMinVal(minValue);
	wavFile.setMaxVal(maxValue);
	wavFile.setNumberOfSamples(sampleNumber);
}

} // namespace audio
} // namespace wtm
