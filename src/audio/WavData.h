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
#ifndef WAV_READER_H_
#define WAV_READER_H_

#include <string>
#include <vector>
#include "audio.h"
#include "Frame.h"

namespace wtm {
namespace audio {

/**
 * WAV header structure
 */
struct WavHeader {
    char                riff[4];        // RIFF Header
    unsigned long       chunkSize;      // RIFF Chunk Size
    char                wave[4];        // WAVE Header

    char                fmt[4];         // FMT header
    unsigned long       subchunk1Size;  // Size of the fmt chunk
    unsigned short      audioFormat;    // Audio format 1=PCM (Other formats are unsupported)
    unsigned short      numOfChan;      // Number of channels 1=Mono, 2=Stereo
    unsigned long       samplesPerSec;  // Sampling Frequency in Hz
    unsigned long       bytesPerSec;    // bytes per second
    unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short      bitsPerSample;  // Number of bits per sample

    // The data below depends on audioFormat, but we work only with PCM cases
    char                data[4];        // DATA header
    unsigned long       subchunk2Size;  // Sampled data length
};

class WavData;
typedef std::auto_ptr<WavData> WavDataPtr;

/**
 * WAV data
 */
class WavData {
public:

	WavData() {
		rawData = new std::vector<raw_t>;
		this->maxVal = 0;
		this->minVal = 0;
		this->numberOfSamples = 0;

		this->frames = new std::vector<Frame*>;
		this->frameLengthMs	= 25;
		this->frameOverlap	= 0.5;
	}

	~WavData() {
		delete rawData;
	}

	static WavDataPtr readFromFile(const std::string& file);

	leght_t getNumberOfSamples() const { return numberOfSamples; }
	void setNumberOfSamples(leght_t numberOfSamples) { this->numberOfSamples = numberOfSamples; }

	raw_t getMaxVal() const { return maxVal; }
	void setMaxVal(raw_t maxVal) { this->maxVal = maxVal; }

	raw_t getMinVal() const { return minVal; }
	void setMinVal(raw_t minVal) { this->minVal = minVal;}

	std::vector<raw_t>& getRawData() const { return *rawData; }

private:
	std::vector<raw_t>*	rawData;
	raw_t				maxVal;
	raw_t				minVal;
	leght_t				numberOfSamples;

	std::vector<Frame*>* frames;
	uint32_t			frameLengthMs;
	double				frameOverlap;	// Overlap between frames - fraction of frame length (0 < overlap < 1)

	static void checkHeader(const WavHeader& wav_header);
	static void readRawData(std::fstream& fs, const WavHeader& wavHeader, WavData& wavFile);
};




} // namespace audio
} // namespace wtm

#endif /* WAV_READER_H_ */
