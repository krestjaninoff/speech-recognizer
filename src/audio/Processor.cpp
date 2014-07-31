#include "Frame.h"
#include "Word.h"
#include <Processor.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

using namespace std;

namespace yazz {
namespace audio {

Processor::Processor(WavData* wavData) {
	this->wavData = wavData;

	this->frames = new vector<Frame*>();
	this->frameToRaw = new map<uint32_t, std::pair<uint32_t, uint32_t> >();
	this->samplesPerFrame = 0;

	this->words = new vector<Word*>();
	this->wordToFrames = new map<uint32_t, std::pair<uint32_t, uint32_t> >();

	this->rmsMax = 0;
	this->wordsThreshold = 0;
}

Processor::~Processor() {
	if (NULL != this->words) {
		vector<Word*>::const_iterator iter;

		for (iter = this->words->begin(); iter != this->words->end(); ++iter) {
			delete *iter;
		}

		delete this->words;
	}

	if (NULL != this->wordToFrames) {
		delete this->wordToFrames;
	}

	if (NULL != this->frames) {
		vector<Frame*>::const_iterator iter;

		for (iter = this->frames->begin(); iter != this->frames->end(); ++iter) {
			delete *iter;
		}

		delete this->frames;
	}

	if (NULL != this->frameToRaw) {
		delete this->frameToRaw;
	}
}

Word* Processor::getAsWholeWord() {
	useAllSamplesAsOneWord();
	return this->words->at(0);
}

void Processor::init() {

	// Init "samples per frame" measure
	uint32_t bytesPerFrame = static_cast<uint32_t>(
			getWavData()->getHeader().bytesPerSec * FRAME_LENGTH / 1000.0);
	uint32_t bytesPerSample = static_cast<uint32_t>(
			getWavData()->getHeader().bitsPerSample / 8);
	this->samplesPerFrame = static_cast<uint32_t>(bytesPerFrame / bytesPerSample);
	assert(this->samplesPerFrame > 0);

	// The main part of splitting
	divideIntoFrames();
}

void Processor::divideIntoFrames() {
	const WavData* wavData = getWavData();

	unsigned int samplesPerNonOverlap =
		static_cast<unsigned int>(this->samplesPerFrame * (1 - FRAME_OVERLAP));
	unsigned int framesCount =
		(getWavData()->getHeader().subchunk2Size / (wavData->getHeader().bitsPerSample / 8))
			/ samplesPerNonOverlap;

	uint32_t indexBegin = 0, indexEnd = 0;
	for (uint32_t frameId = 0, size = wavData->getNumberOfSamples(); frameId < framesCount;
			++frameId) {

		indexBegin = frameId * samplesPerNonOverlap;
		indexEnd = indexBegin + samplesPerFrame;
		if (indexEnd < size) {

			Frame* frame = new Frame(frameId);
			frame->init(getWavData()->getRawData(), getWavData()->getNormalizedData(),
					indexBegin, indexEnd);

			this->frames->insert(this->frames->begin() + frameId, frame);
			this->frameToRaw->insert(std::make_pair(frameId, make_pair(indexBegin, indexEnd)));
		} else {
			break;
		}
	}
}

void Processor::divideIntoWords() {
	assert(frames->size() > 10);

	// Let's find silence threshold
	bool hasSilence = findSilenceThreshold();

	// Divide frames into words
	uint32_t wordId = -1;
	long firstFrameInCurrentWordNumber = -1;
	Word* lastWord = 0;

	if (hasSilence) {
		for (vector<Frame*>::const_iterator frame = this->frames->begin();
				frame != this->frames->end(); ++frame) {

			// Got a sound
			if ((*frame)->getRms() > this->wordsThreshold) {

				if (-1 == firstFrameInCurrentWordNumber) {
					firstFrameInCurrentWordNumber = (*frame)->getId();
					DEBUG("Word started at frame %d", (int) firstFrameInCurrentWordNumber);
				}

			// Got silence
			} else {
				if (firstFrameInCurrentWordNumber >= 0) {

					// Let's find distance between start of the current word and end of the previous word
					processSilence(frame, lastWord,	firstFrameInCurrentWordNumber, wordId);
				}
			}
		}

		// Clean up short words
		cleanUpWords();

	// There is no any silence in the sound
	} else {
		useAllSamplesAsOneWord();
	}

	// If we have only one word let's consider whole sample
	if (hasSilence && 1 == this->words->size()) {
		useAllSamplesAsOneWord();
	}
}

uint32_t Processor::processSilence(vector<Frame*>::const_iterator frame,
		Word*& lastWord, long & firstFrameInCurrentWordNumber,	uint32_t& wordId) {

	// Let's find distance between start of the current word and end of the previous word
	uint32_t distance = 0;
	if (0 != lastWord) {

		uint32_t lastFrameInPreviousWordNumber = (*this->wordToFrames)[lastWord->getId()].second;
		distance = firstFrameInCurrentWordNumber - lastFrameInPreviousWordNumber;
	}

	// We have a new word
	if (0 == lastWord || distance >= WORDS_MIN_DISTANCE) {
		wordId++;
		lastWord = new Word(wordId);

		this->wordToFrames->insert(	make_pair(lastWord->getId(),
						make_pair(firstFrameInCurrentWordNumber, (*frame)->getId())));
		this->words->push_back(lastWord);

		DEBUG("We have a word %d (%d - %d)", (int ) lastWord->getId(),
				(int ) firstFrameInCurrentWordNumber, (int ) (*frame)->getId());

	// We need to add the current word to the previous one
	} else if (0 != lastWord && distance < WORDS_MIN_DISTANCE) {

		// Compute RMS for current word
		double currentWordRms = 0;
		for (uint32_t i = firstFrameInCurrentWordNumber; i < (*frame)->getId();	i++) {
			currentWordRms += this->frames->at(i)->getRms();
		}
		currentWordRms /= (*frame)->getId() - firstFrameInCurrentWordNumber;

		// Add the word only if it has valuable RMS
		if (currentWordRms > this->wordsThreshold * 2) {
			uint32_t firstFrameInPreviousWordNumber =
					(*this->wordToFrames)[lastWord->getId()].first;

			this->wordToFrames->erase(lastWord->getId());
			this->wordToFrames->insert(make_pair(lastWord->getId(),
					make_pair(firstFrameInPreviousWordNumber, (*frame)->getId())));

			DEBUG("Word %d will be extended (%d - %d)",
					(int) lastWord->getId(),
					(int) (*this->wordToFrames)[lastWord->getId()].first,
					(int) (*frame)->getId());
		}
	}

	firstFrameInCurrentWordNumber = -1;
	return distance;
}

// Clean up short words
void Processor::cleanUpWords() {

	for (vector<Word*>::iterator word = this->words->begin();
			word != this->words->end();) {

		if (getFramesCount(**word) < WORD_MIN_SIZE) {
			DEBUG("Word %d is too short and will be avoided",
					(int ) (*word)->getId());

			this->wordToFrames->erase((*word)->getId());
			word = this->words->erase(word);

		} else {
			++word;
		}
	}
}

// @unused
void Processor::useAllSamplesAsOneWord() {
	this->words->clear();
	this->wordToFrames->clear();

	Word* theWord = new Word(0);

	this->wordToFrames->insert(	make_pair(theWord->getId(), make_pair(
			this->frames->at(0)->getId(),
			this->frames->at(this->frames->size() - 1)->getId())));
	this->words->push_back(theWord);

	DEBUG("Seems we have only one word in the sample... All frames will be added into the word!");
}

bool Processor::findSilenceThreshold() {

	// Let's find max and min rms/entropy
	double rms, rmsMax, rmsSilence = 0;
	rms = rmsMax = this->frames->at(0)->getRms();

	// Try to guess the best threshold value
	bool hasSilence = false;
	uint32_t cnt = 0;
	for (vector<Frame*>::const_iterator frame = this->frames->begin();
			frame != this->frames->end(); ++frame) {

		rms = (*frame)->getRms();
		rmsMax = std::max(rmsMax, rms);

		if ((*frame)->getEntropy() < ENTROPY_THRESHOLD) {
			hasSilence = true;
			rmsSilence += (*frame)->getRms();
			cnt++;
		}
	}
	rmsSilence /= cnt;

	this->rmsMax = rmsMax;
	this->wordsThreshold = rmsSilence * 2;

	return hasSilence;
}

// @unused
void Processor::initMfcc(Word& word) {

	uint32_t firstId = (*this->wordToFrames)[word.getId()].first;
	uint32_t lastId = (*this->wordToFrames)[word.getId()].second;

	uint32_t framesCnt = lastId - firstId + 1;
	double* mfcc = new double[MFCC_SIZE * framesCnt];

	for (uint32_t i = 0; i < framesCnt; i++) {
		uint32_t rawBegin = (*this->frameToRaw)[firstId + i].first;
		uint32_t rawFinsh = (*this->frameToRaw)[firstId + i].second;

		double* frameMfcc = this->frames->at(firstId + i)->initMFCC(
				getWavData()->getNormalizedData(), rawBegin, rawFinsh,
				getWavData()->getHeader().samplesPerSec);

		for (uint32_t j = 0; j < MFCC_SIZE; j++) {
			mfcc[i * MFCC_SIZE  + j] = frameMfcc[j];
		}
	}

	word.setMfcc(mfcc, MFCC_SIZE * framesCnt);
}

void Processor::saveWordAsAudio(const std::string& file, const Word& word) const {

	// number of data bytes in the resulting wave file
	unsigned int samplesPerNonOverlap =
			static_cast<unsigned int>(this->samplesPerFrame * (1 - FRAME_OVERLAP));
	unsigned int waveSize = getFramesCount(word) * samplesPerNonOverlap * sizeof(raw_t);

	// prepare a new header and write it to file stream
	WavHeader headerNew;
	strncpy(headerNew.riff, getWavData()->getHeader().riff, 4);
	headerNew.chunkSize = waveSize + sizeof(WavHeader);
	strncpy(headerNew.wave, getWavData()->getHeader().wave, 4);
	strncpy(headerNew.fmt, getWavData()->getHeader().fmt, 4);
	headerNew.subchunk1Size = getWavData()->getHeader().subchunk1Size;
	headerNew.audioFormat = getWavData()->getHeader().audioFormat;
	headerNew.numOfChan = 1;
	headerNew.samplesPerSec = getWavData()->getHeader().samplesPerSec;
	headerNew.bytesPerSec = getWavData()->getHeader().samplesPerSec * sizeof(raw_t);
	headerNew.blockAlign = sizeof(raw_t);
	headerNew.bitsPerSample = sizeof(raw_t) * 8;
	strncpy(headerNew.data, getWavData()->getHeader().data, 4);
	headerNew.subchunk2Size = waveSize;

	std::fstream fs;
	fs.open(file.c_str(), std::ios::out | std::ios::binary);
	fs.write((char*)&headerNew, sizeof(WavHeader));

	raw_t* data = new raw_t[waveSize / sizeof(raw_t)];

	int frameNumber = 0;
	uint32_t frameStart = -1;
	for (uint32_t currentFrame = (*this->wordToFrames)[word.getId()].first;
			currentFrame < (*this->wordToFrames)[word.getId()].second; currentFrame++) {
		frameStart = (*this->frameToRaw)[currentFrame].first;

		for (uint32_t i = 0; i < samplesPerNonOverlap; i++) {
			data[frameNumber * samplesPerNonOverlap + i ] =
					this->wavData->getRawData()[frameStart + i];
		}

		frameNumber++;
	}

	fs.write((char*)data, waveSize);
	fs.close();
	delete [] data;
}

bool Processor::isPartOfAWord(const Frame& frame) const {
	bool isPartOfWord = false;

	std::map<uint32_t, std::pair<uint32_t, uint32_t> >::const_iterator word;
	for (word = this->wordToFrames->begin(); word != this->wordToFrames->end(); ++word) {

		if (word->second.first <= frame.getId() && frame.getId() <= word->second.second) {
			isPartOfWord = true;
			break;
		}
	}

	return isPartOfWord;
}

uint32_t Processor::getFramesCount(const Word& word) const {

	 uint32_t cnt = (*this->wordToFrames)[word.getId()].second -
			 (*this->wordToFrames)[word.getId()].first;
	 return cnt;
}

} /* namespace audio */
} /* namespace yazz */
