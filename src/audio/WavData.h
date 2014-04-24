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
#include "Word.h"

namespace wtm {
namespace audio {

/**
 * WAV header structure
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
typedef std::auto_ptr<WavData> WavDataPtr;

/**
 * WAV data
 */
class WavData {
public:

	~WavData() {
		if (this->rawData) {
			delete this->rawData;
		}

		if (this->frames) {
			delete this->frames;
		}

		if (this->words) {
			delete this->words;
		}
	}

	static WavDataPtr readFromFile(const std::string& file);
	void init();

	length_t getNumberOfSamples() const { return numberOfSamples; }
	void setNumberOfSamples(length_t numberOfSamples) { this->numberOfSamples = numberOfSamples; }

	raw_t getMaxVal() const { return maxVal; }
	void setMaxVal(raw_t maxVal) { this->maxVal = maxVal; }

	raw_t getMinVal() const { return minVal; }
	void setMinVal(raw_t minVal) { this->minVal = minVal; }

	double getWordsThreshold() const { return this->wordsThreshold; }
	double getMaRMSMax() const { return this->maRmsMax; }

	const WavHeader& getHeader() const { return header; }
	const std::vector<raw_t>& getRawData() const { return *rawData; }
	const std::vector<Frame*>& getFrames() const { return *frames; }
	const std::vector<Word*>& getWords() const { return *words; }

	void saveToFile(const std::string& file, const Word& word) const;

private:
	WavHeader               header;

	std::vector<raw_t>*     rawData;
	raw_t                   maxVal;
	raw_t                   minVal;
	length_t                numberOfSamples;

	std::vector<Frame*>*    frames;
	length_t                samplesPerFrame;

	std::vector<Word*>*     words;
	raw_t					maRmsMax;
	double                 wordsThreshold;

	WavData(WavHeader header) {
		this->header = header;

		this->rawData = new std::vector<raw_t>;
		this->maxVal = 0;
		this->minVal = 0;
		this->numberOfSamples = 0;

		this->frames = new std::vector<Frame*>;
		this->samplesPerFrame = 0;

		this->words = new std::vector<Word*>;
		this->maRmsMax = 0;
		this->wordsThreshold = 0;
	}

	static void checkHeader(const WavHeader& wav_header);
	static void readRawData(std::fstream& fs, const WavHeader& wavHeader, WavData& wavFile);

	void divideIntoFrames();

	double getThresholdCandidate(double min, double avg, double max);
	void divideIntoWords();
};

} // namespace audio
} // namespace wtm

#endif /* WAV_READER_H_ */
