#ifndef SPLITTER_H_
#define SPLITTER_H_

#include <vector>
#include <map>
#include <utility>
#include "audio.h"
#include "WavData.h"
#include "Frame.h"
#include "Word.h"

namespace wtm {
namespace audio {

/**
 * Splitter is a mediator which splits raw data into frames and words
 * and keeps relations between these objects
 */
class Splitter {
public:

	Splitter(WavData* wavData) {
		this->wavData = wavData;

		this->frames = new std::vector<Frame*>();
		this->frameToRaw = new std::map<length_t, std::pair<length_t, length_t>>();
		this->samplesPerFrame = 0;

		this->words = new std::vector<Word*>();
		this->wordToFrames = new std::map<length_t, std::pair<length_t, length_t>>();
		this->maRmsMax = 0;
		this->wordsThreshold = 0;
	}

	~Splitter() {
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

	const WavData* getWavData() const { return this->wavData; }
	const std::vector<Frame*>* getFrames() const { return this->frames; }
	const std::vector<Word*>* getWords() const { return this->words; }

	double getWordsThreshold() const { return this->wordsThreshold; }
	double getMaRMSMax() const { return this->maRmsMax; }

	const std::vector<Frame*>* getFramesOfWord(length_t wordId) const;
	length_t getFramesCntForWOrd(length_t wordId) const;
	bool isPartOfAWord(const Frame* frame) const;

	void saveWordAsAudio(const std::string& file, const Word& word) const;

private:
	WavData* wavData;

	std::vector<Frame*>* frames;
	std::map<length_t, std::pair<length_t, length_t>>* frameToRaw;
	length_t samplesPerFrame;

	std::vector<Word*>* words;
	std::map<length_t, std::pair<length_t, length_t>>* wordToFrames;
	raw_t maRmsMax;
	double wordsThreshold;

	void divideIntoFrames();

	double getThresholdCandidate(double min, double avg, double max);
	void divideIntoWords();
};

typedef std::unique_ptr<Splitter> SplitterPtr;

} /* namespace audio */
} /* namespace wtm */

#endif /* SPLITTER_H_ */
