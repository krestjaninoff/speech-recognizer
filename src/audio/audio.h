/*
 * Common objects for work with audio
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <string>
#include <list>
#include <memory>
#include <cstdlib>
#include <stdint.h>

namespace wtm {
namespace audio {

/**
 * WAV header
 */
typedef struct TWavHeader {
    char                 riff[4];        // RIFF Header
    unsigned long       chunkSize;      // RIFF Chunk Size
    char                 wave[4];        // WAVE Header

    char                 fmt[4];         // FMT header
    unsigned long       subchunk1Size;  // Size of the fmt chunk
    unsigned short      audioFormat;    // Audio format 1=PCM, 6=mulaw, 7=alaw,
    									  // 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    unsigned short      numOfChan;      // Number of channels 1=Mono 2=Sterio
    unsigned long       samplesPerSec;  // Sampling Frequency in Hz
    unsigned long       bytesPerSec;    // bytes per second
    unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short      bitsPerSample;  // Number of bits per sample

    char                 subchunk2ID[4]; // DATA header
    unsigned long       subchunk2Size;  // Sampled data length

} TWavHeader;

/**
 * RAW data
 */
typedef uint8_t raw_t;

/**
 * WAV data
 */
class TWavData {
public:

	TWavData() {
		this->maxVal = 0;
		this->minVal = 0;
		this->duration = 0;

		rawData = new std::list<raw_t>;
	}

	~TWavData() {
		delete rawData;
	}

	uint32_t getDuration() const {
		return duration;
	}
	void setDuration(uint32_t duration) {
		this->duration = duration;
	}

	raw_t getMaxVal() const {
		return maxVal;
	}
	void setMaxVal(raw_t maxVal) {
		this->maxVal = maxVal;
	}

	raw_t getMinVal() const {
		return minVal;
	}
	void setMinVal(raw_t minVal) {
		this->minVal = minVal;
	}

	std::list<raw_t>* getRawData() const {
		return rawData;
	}

private:
	std::list<raw_t>*	rawData;
	raw_t				maxVal;
	raw_t				minVal;
	uint32_t			duration;
};

typedef std::auto_ptr<TWavData> TWavDataPtr;

} // namespace audio
} // namespace wtm

#endif /* AUDIO_H_ */
