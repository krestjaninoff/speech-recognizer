/**
 * Represents WAV file data
 *
 * Currently supports only PCM format.
 *
 * @see http://en.wikipedia.org/wiki/WAV
 * @see http://en.wikipedia.org/wiki/Linear_pulse-code_modulation
 * @see https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 * @see https://code.google.com/p/aquila/source
 */
#ifndef WAV_DATA_H_
#define WAV_DATA_H_

#include <string>
#include <vector>
#include "audio.h"
#include "Frame.h"
#include "Word.h"

namespace yazz {
namespace audio {

/**
 * WAV header
 */
struct WavHeader {
    char               riff[4];        // RIFF Header
    unsigned long     chunkSize;      // RIFF Chunk Size
    char               wave[4];        // WAVE Header

    char               fmt[4];         // FMT header
    unsigned long     subchunk1Size;  // Size of the fmt chunk
    unsigned short    audioFormat;    // Audio format 1=PCM (Other formats are unsupported)
    unsigned short    numOfChan;      // Number of channels 1=Mono, 2=Stereo
    unsigned long     samplesPerSec;  // Sampling Frequency in Hz
    unsigned long     bytesPerSec;    // bytes per second
    unsigned short    blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short    bitsPerSample;  // Number of bits per sample

    // The data below depends on audioFormat, but we work only with PCM cases
    char               data[4];        // DATA header
    unsigned long     subchunk2Size;  // Sampled data length
};

class WavData;

/**
 * WAV data
 */
class WavData {
public:

	~WavData() {
		if (NULL != this->rawData) {
			delete this->rawData;
		}
		if (NULL != this->normalizaedData) {
			delete this->normalizaedData;
		}
	}

	static WavData* readFromFile(const std::string& file);

	uint32_t getNumberOfSamples() const { return numberOfSamples; }
	void setNumberOfSamples(uint32_t numberOfSamples) { this->numberOfSamples = numberOfSamples; }

	raw_t getMaxVal() const { return maxVal; }
	void setMaxVal(raw_t maxVal) { this->maxVal = maxVal; }

	raw_t getMinVal() const { return minVal; }
	void setMinVal(raw_t minVal) { this->minVal = minVal; }

	const WavHeader& getHeader() const { return header; }
	raw_t* getRawData() const { return rawData; }
	const double* getNormalizedData() const { return normalizaedData; }

private:
	WavHeader		header;
	raw_t*			rawData;
	double*			normalizaedData;

	raw_t			maxVal;
	raw_t			minVal;
	uint32_t		numberOfSamples;

	WavData(WavHeader header) {
		this->header = header;
		this->rawData = NULL;
		this->normalizaedData = NULL;

		this->maxVal = 0;
		this->minVal = 0;
		this->numberOfSamples = 0;
	}

	static bool checkHeader(const WavHeader& wavHeader);
	static void readData(std::fstream& fs, const WavHeader& wavHeader, WavData& wavFile);
};

} // namespace audio
} // namespace yazz

#endif /* WAV_DATA_H_ */
