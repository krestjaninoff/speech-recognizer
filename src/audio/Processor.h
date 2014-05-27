#ifndef Processor_H_
#define Processor_H_

#include "../common.h"
#include <vector>
#include <map>
#include <utility>
#include "audio.h"
#include "WavData.h"
#include "Frame.h"
#include "Word.h"
#include <memory>

namespace yazz {
namespace audio {

/**
 * Processor is a mediator which splits raw data into frames and words
 * and keeps relations between these objects
 */
class Processor {
public:

	Processor(WavData* wavData) {
		this->wavData = wavData;

		this->frames = new std::vector<Frame*>();
		this->frameToRaw = new std::map<uint32_t, std::pair<uint32_t, uint32_t> >();
		this->samplesPerFrame = 0;

		this->words = new std::vector<Word*>();
		this->wordToFrames = new std::map<uint32_t, std::pair<uint32_t, uint32_t> >();
		this->maRmsMax = 0;
		this->wordsThreshold = 0;
	}

	~Processor() {
		if (this->words) {
			delete this->words;
			delete this->wordToFrames;
		}

		if (this->frames) {
			delete this->frames;
			delete this->frameToRaw;
		}

		delete this->wavData;
	}

	void split();
	void initMfcc(Word& word);

	const WavData* getWavData() const { return this->wavData; }
	const std::vector<Frame*>* getFrames() const { return this->frames; }
	const std::vector<Word*>* getWords() const { return this->words; }

	double getWordsThreshold() const { return this->wordsThreshold; }
	double getMaRMSMax() const { return this->maRmsMax; }

	bool isPartOfAWord(const Frame& frame) const;
	uint32_t getFramesCount(const Word& word) const;

	void saveWordAsAudio(const std::string& file, const Word& word) const;

private:
	WavData* wavData;

	std::vector<Frame*>* frames;
	std::map<uint32_t, std::pair<uint32_t, uint32_t> >* frameToRaw;
	uint32_t samplesPerFrame;

	std::vector<Word*>* words;
	std::map<uint32_t, std::pair<uint32_t, uint32_t> >* wordToFrames;
	raw_t maRmsMax;
	double wordsThreshold;

	void divideIntoFrames();

	double getThresholdCandidate(double min, double avg, double max);
	void divideIntoWords();
};

//typedef std::unique_ptr<Processor> ProcessorPtr;
typedef Processor* ProcessorPtr;

} /* namespace audio */
} /* namespace yazz */

#endif /* Processor_H_ */
