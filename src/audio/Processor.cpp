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
#include <string>
#include <cassert>
#include <Processor.h>

using namespace std;

namespace yazz {
namespace audio {

void Processor::split() {

	// Init "samples per frame" measure
	uint32_t bytesPerFrame = static_cast<uint32_t>(
			getWavData()->getHeader().bytesPerSec * FRAME_LENGTH / 1000.0);
	uint32_t bytesPerSample = static_cast<uint32_t>(
			getWavData()->getHeader().bitsPerSample / 8);
	this->samplesPerFrame = static_cast<uint32_t>(bytesPerFrame / bytesPerSample);
	assert(this->samplesPerFrame > 0);

	// The main part of splitting
	divideIntoFrames();
	divideIntoWords();
}

void Processor::divideIntoFrames() {
	const WavData* wavData = getWavData();

	unsigned int samplesPerNonOverlap =
		static_cast<unsigned int>(this->samplesPerFrame * (1 - FRAME_OVERLAP));
	unsigned int framesCount =
		(getWavData()->getHeader().subchunk2Size / (wavData->getHeader().bitsPerSample / 8))
			/ samplesPerNonOverlap;

	this->frames->reserve(framesCount);

	uint32_t indexBegin = 0, indexEnd = 0;
	for (uint32_t frameId = 0, size = wavData->getRawData()->size(); frameId < framesCount;
			++frameId) {

		indexBegin = frameId * samplesPerNonOverlap;
		indexEnd = indexBegin + samplesPerFrame;
		if (indexEnd < size) {

			Frame* frame = new Frame(frameId);
			frame->init(*getWavData()->getRawData(), indexBegin, indexEnd,
					wavData->getMinVal(), wavData->getMaxVal());

			this->frames->insert(this->frames->begin() + frameId, frame);
			this->frameToRaw->insert(std::make_pair(frameId, make_pair(indexBegin, indexEnd)));
		} else {
			break;
		}
	}
}

void Processor::divideIntoWords() {
	assert(frames->size() > 10);

	//double entropy, entropyMin, entropyMax;
	double rms, rmsMin, rmsMax;

	// Let's find max and min rms/entropy
	rms = rmsMin = rmsMax = this->frames->at(0)->getRms();
	//entropy = entropyMax = entropyMin = this->frames->at(0)->getEntropy();

	uint32_t iFrame;
	for (iFrame = 1; iFrame < this->frames->size(); ++iFrame) {

		rms = this->frames->at(iFrame)->getRms();
		//rmsMin = std::min(rmsMin, rms);
		rmsMax = std::max(rmsMax, rms);

		//entropy = this->frames->at(iFrame)->getEntropy();
		//entropyMin = std::min(entropyMin, entropy);
		//entropyMax = std::max(entropyMax, entropy);
	}
	this->maRmsMax = rmsMax;

	// Tries to guess the best threshold value
	// double threshold = (entropyMax - entropyMin) / 2. + 0.1 * entropyMin;
	double threshold = ENTROPY_THRESHOLD;
	this->wordsThreshold = threshold;

	// Divide frames into words
	uint32_t wordId = -1;
	long firstFrameInCurrentWordNumber = -1;
	Word* lastWord = 0;

	DEBUG("_");
	for (vector<Frame*>::const_iterator frame = this->frames->begin();
			frame != this->frames->end(); ++frame) {

		// Got a sound
		if ((*frame)->getEntropy() > threshold) {

			if (-1 == firstFrameInCurrentWordNumber) {
				firstFrameInCurrentWordNumber = (*frame)->getId();
				DEBUG("Word started at frame %d", (int) firstFrameInCurrentWordNumber);
			}

		// Got silence
		} else {
			if (firstFrameInCurrentWordNumber >= 0) {

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

					this->wordToFrames->insert(make_pair(lastWord->getId(),
							make_pair(firstFrameInCurrentWordNumber, (*frame)->getId())));
					this->words->push_back(lastWord);

					DEBUG("We have a word %d (%d - %d)", (int) lastWord->getId(),
							(int) firstFrameInCurrentWordNumber, (int) (*frame)->getId());

				// We need to add the current word to the previous one
				} else if (0 != lastWord && distance < WORDS_MIN_DISTANCE) {
					uint32_t firstFrameInPreviousWordNumber =
							(*this->wordToFrames)[lastWord->getId()].first;

					this->wordToFrames->erase(lastWord->getId());
					this->wordToFrames->insert(make_pair(lastWord->getId(),
							make_pair(firstFrameInPreviousWordNumber, (*frame)->getId())));

					DEBUG("Word %d will be extended (%d - %d)", (int) lastWord->getId(),
							(int) (*this->wordToFrames)[lastWord->getId()].first, (int) (*frame)->getId());
				}

				firstFrameInCurrentWordNumber = -1;
			}
		}
	}
	DEBUG("_");

	// Clean up short words
	DEBUG("_");
	for (vector<Word*>::iterator word = this->words->begin();
			word != this->words->end();) {

		if (getFramesCount(**word) < WORD_MIN_SIZE) {
			DEBUG("Word %d is too short and will be avoided", (int) (*word)->getId());

			this->wordToFrames->erase((*word)->getId());
			this->words->erase(word);
		} else {
			 ++word;
		}
	}
	DEBUG("_");
}

void Processor::initMfcc(Word& word) {

	uint32_t firstId = (*this->wordToFrames)[word.getId()].first;
	uint32_t lastId = (*this->wordToFrames)[word.getId()].second;

	uint32_t framesCnt = lastId - firstId + 1;
	double* mfcc = new double[MFCC_SIZE * framesCnt];

	for (uint32_t i = 0; i < framesCnt; i++) {
		uint32_t rawBegin = (*this->frameToRaw)[firstId + i].first;
		uint32_t rawFinsh = (*this->frameToRaw)[firstId + i].second;

		double* frameMfcc = this->frames->at(firstId + i)->initMFCC(
				*getWavData()->getRawData(), rawBegin, rawFinsh,
				getWavData()->getHeader().samplesPerSec);

		for (uint32_t j = 0; j < MFCC_SIZE; j++) {
			mfcc[i * MFCC_SIZE  + j] = frameMfcc[j];
		}
	}

	word.setMfcc(mfcc, MFCC_SIZE * framesCnt);

	if (DEBUG_ENABLED) {
		DEBUG("MFCC for %d are: ", (int) word.getId());
		for (uint32_t i = 0; i < word.getMfccSize(); i++) {
			//DEBUG("%f ", word.getMfcc()[i]);
			cout << word.getMfcc()[i] << endl;
		}
		DEBUG("_");
	}
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
					this->wavData->getRawData()->at(frameStart + i);
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

	 uint32_t cnt = (*this->wordToFrames)[word.getId()].second - (*this->wordToFrames)[word.getId()].first;
	 return cnt;
}

} /* namespace audio */
} /* namespace yazz */
