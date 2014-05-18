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

namespace wtm {
namespace audio {

void Processor::split() {

	// Init "samples per frame" measure
	length_t bytesPerFrame = static_cast<length_t>(
			getWavData()->getHeader().bytesPerSec * FRAME_LENGTH / 1000.0);
	length_t bytesPerSample = static_cast<uint32_t>(
			getWavData()->getHeader().bitsPerSample / 8);
	this->samplesPerFrame = static_cast<length_t>(bytesPerFrame / bytesPerSample);
	assert(this->samplesPerFrame > 0);

	// The main part of splitting
	divideIntoFrames();
	divideIntoWords();
}

void Processor::divideIntoFrames() {

	unsigned int samplesPerNonOverlap =
		static_cast<unsigned int>(this->samplesPerFrame * (1 - FRAME_OVERLAP));
	unsigned int framesCount =
		(getWavData()->getHeader().subchunk2Size / (getWavData()->getHeader().bitsPerSample / 8))
			/ samplesPerNonOverlap;

	this->frames->reserve(framesCount);

	length_t indexBegin = 0, indexEnd = 0;
	for (length_t frameId = 0, size = getWavData()->getRawData()->size(); frameId < framesCount;
			++frameId) {

		indexBegin = frameId * samplesPerNonOverlap;
		indexEnd = indexBegin + samplesPerFrame;
		if (indexEnd < size) {

			Frame* frame = new Frame(frameId);
			frame->init(*getWavData()->getRawData(), indexBegin, indexEnd);

			this->frames->insert(this->frames->begin() + frameId, frame);
			this->frameToRaw->insert(std::make_pair(frameId, make_pair(indexBegin, indexEnd)));
		} else {
			break;
		}
	}
}

void Processor::divideIntoWords() {
	assert(frames->size() > 10);

	double maMin = 0;
	double maAvg = 0;
	double maMax = 0;
	double ma;

	// Let's use Moving Average value to avoid spikes
	unsigned short maShift = MOVING_AVERAGE_SIZE / 2;
	maAvg = maMin = this->frames->at(0)->getRms();
	length_t iFrame;
	for (iFrame = maShift; iFrame < this->frames->size() - maShift; ++iFrame) {

		ma = 0;
		for (unsigned short iMa = iFrame - maShift; iMa <= iFrame + maShift; iMa++) {
			ma += this->frames->at(iMa)->getRms();
		}
		ma /= MOVING_AVERAGE_SIZE;
		this->frames->at(iFrame)->setMaRms(ma);

		if (maMin > ma) {
			maMin = ma;
		}
		if (ma > maMax) {
			maMax = ma;
		}

		maAvg += ma;
	}
	maAvg /= iFrame;
	this->maRmsMax = maMax;

	// A little hack to calculate bound values
	for (length_t iFrame = 0; iFrame < maShift; ++iFrame) {
		this->frames->at(iFrame)->setMaRms(this->frames->at(iFrame)->getRms());
		this->frames->at(this->frames->size() - 1 - iFrame)->setMaRms(
				this->frames->at(this->frames->size() - 1 - iFrame)->getRms());
	}

	// Tries to guess the best threshold value
	double thresholdCandidate = getThresholdCandidate(maMin, maAvg, maMax);
	this->wordsThreshold = thresholdCandidate;

	// If max value greater than min value more then 50% then we have the "silence" threshold.
	// Otherwise, let's think that we have only one word.
	double threshold = 0;
	length_t wordId = -1;

	if (maMax * 0.5 > maMin) {
		threshold = thresholdCandidate;

		// Divide frames into words
		long firstFrameInCurrentWordNumber = -1;
		Word* lastWord = 0;

		DEBUG("_");
		for (vector<Frame*>::const_iterator frame = this->frames->begin();
				frame != this->frames->end(); ++frame) {

			// Got a sound
			if ((*frame)->getMaRms() > threshold) {

				if (-1 == firstFrameInCurrentWordNumber) {
					firstFrameInCurrentWordNumber = (*frame)->getId();
					DEBUG("Word started at frame %d", (int) firstFrameInCurrentWordNumber);
				}

			// Got silence
			} else {
				if (firstFrameInCurrentWordNumber >= 0) {

					// Let's find distance between start of the current word and end of the previous word
					length_t distance = 0;
					if (0 != lastWord) {

						length_t lastFrameInPreviousWordNumber = this->wordToFrames->at(lastWord->getId()).second;
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
						length_t firstFrameInPreviousWordNumber = wordToFrames->at(lastWord->getId()).first;

						this->wordToFrames->erase(lastWord->getId());
						this->wordToFrames->insert(make_pair(lastWord->getId(),
								make_pair(firstFrameInPreviousWordNumber, (*frame)->getId())));

						DEBUG("Word %d will be extended (%d - %d)", (int) lastWord->getId(),
								(int) wordToFrames->at(lastWord->getId()).first, (int) (*frame)->getId());
					}

					firstFrameInCurrentWordNumber = -1;
				}
			}
		}
		DEBUG("_");

		// Clean up short words
		DEBUG("_");
		for (vector<Word*>::iterator word = this->words->begin();
				word != this->words->end(); ++word) {

			if (getFramesCount(**word) < WORD_MIN_SIZE) {
				DEBUG("Word %d is too short and will be avoided", (int) (*word)->getId());

				this->wordToFrames->erase((*word)->getId());
				this->words->erase(word);
			}
		}
		DEBUG("_");


	// Seems we have only one word
	} else {

		this->words->push_back(new Word(wordId));
		this->wordToFrames->insert(make_pair(wordId,
				make_pair(frames->at(0)->getId(), frames->at(frames->size() - 1)->getId())));
	}
}

/**
 * Determination of silence threshold
 *
 * Method divides data into 3 clusters (using something like k-means algorithm).
 * The cluster center of "Min" cluster is used as a threshold candidate.
 */
double Processor::getThresholdCandidate(double maMin, double maAvg, double maMax) {
	UNUSED(maAvg);
	short currIter = 0, maxIterCnt = 30;
	bool isCenterChanged = true;

	// Init clusters
	double minClusterCenter = maMin;
	double minClusterCenterNew = 0;
	std::vector<Frame*>* minCluster = new std::vector<Frame*>();

	// Just an empirical solution
	double avgClusterCenter = maMax / 2;
	double avgClusterCenterNew = 0;
	std::vector<Frame*>* avgCluster = new std::vector<Frame*>();

	double maxClusterCenter = maMax;
	double maxClusterCenterNew = 0;
	std::vector<Frame*>* maxCluster = new std::vector<Frame*>();

	double maRms;
	for (vector<Frame*>::const_iterator frame = this->frames->begin();
		frame != this->frames->end(); ++frame) {

		maRms = (*frame)->getMaRms();

		if (fabs(maRms - minClusterCenter) < fabs(maRms - avgClusterCenter)
				&& fabs(maRms - minClusterCenter) < fabs(maRms - maxClusterCenter)) {
			minCluster->push_back(*frame);

		} else if (fabs(maRms - avgClusterCenter) < fabs(maRms - minClusterCenter)
				&& fabs(maRms - avgClusterCenter) < fabs(maRms - maxClusterCenter)) {
			avgCluster->push_back(*frame);

		} else {
			maxCluster->push_back(*frame);
		}
	}

	// Iterate
	while (currIter < maxIterCnt && isCenterChanged) {

		//DEBUG("Min center: %f, size: %d", minClusterCenter, minCluster->size());
		//DEBUG("Avg center: %f, size: %d", avgClusterCenter, avgCluster->size());
		//DEBUG("Max center: %f, size: %d", maxClusterCenter, maxCluster->size());
		//DEBUG("_");

		// Calculates new cluster centers
		if (minCluster->size() > 0) {
			minClusterCenterNew = minCluster->at(0)->getMaRms();

			for (vector<Frame*>::const_iterator frame = minCluster->begin();
						frame != minCluster->end(); ++frame) {
				minClusterCenterNew += (*frame)->getMaRms();
			}
			minClusterCenterNew /= minCluster->size();

		} else {
			break;
		}

		if (avgCluster->size() > 0) {
			avgClusterCenterNew = avgCluster->at(0)->getMaRms();

			for (vector<Frame*>::const_iterator frame = avgCluster->begin();
						frame != avgCluster->end(); ++frame) {
				avgClusterCenterNew += (*frame)->getMaRms();
			}
			avgClusterCenterNew /= avgCluster->size();

		} else {
			break;
		}

		if (maxCluster->size() > 0) {
			maxClusterCenterNew = maxCluster->at(0)->getMaRms();

			for (vector<Frame*>::const_iterator frame = maxCluster->begin();
						frame != maxCluster->end(); ++frame) {
				maxClusterCenterNew += (*frame)->getMaRms();
			}
			maxClusterCenterNew /= maxCluster->size();

		} else {
			break;
		}

		// Check if clusters centers changed
		if (fabs(minClusterCenterNew - minClusterCenter) < numeric_limits<double>::epsilon()
				&& fabs(avgClusterCenterNew - avgClusterCenter) < numeric_limits<double>::epsilon()
				&& fabs(maxClusterCenterNew - maxClusterCenter) < numeric_limits<double>::epsilon()) {
			isCenterChanged = false;
			break;
		}

		// Update clusters centers
		minClusterCenter = minClusterCenterNew;
		avgClusterCenter = avgClusterCenterNew;
		maxClusterCenter = maxClusterCenterNew;

		// Rebuild clusters
		minCluster->clear();
		avgCluster->clear();
		maxCluster->clear();

		for (vector<Frame*>::const_iterator frame = this->frames->begin();
				frame != this->frames->end(); ++frame) {

			if (fabs((*frame)->getMaRms() - minClusterCenter) < fabs((*frame)->getMaRms() - avgClusterCenter)
					&& fabs((*frame)->getMaRms() - minClusterCenter) < fabs((*frame)->getMaRms() - maxClusterCenter)) {

				minCluster->push_back(*frame);

			} else if (fabs((*frame)->getMaRms() - avgClusterCenter) < fabs((*frame)->getMaRms() - minClusterCenter)
					&& fabs((*frame)->getMaRms() - avgClusterCenter) < fabs((*frame)->getMaRms() - maxClusterCenter)) {

				avgCluster->push_back(*frame);

			} else {
				maxCluster->push_back(*frame);
			}
		}

		currIter++;
	}

	double thresholdCandidate = minClusterCenter / 2;
	DEBUG("Threshold candidate: %f", thresholdCandidate);

	delete minCluster;
	delete avgCluster;
	delete maxCluster;

	return thresholdCandidate;
}

void Processor::initMfcc(Word& word) {

	length_t firstId = this->wordToFrames->at(word.getId()).first;
	length_t lastId = this->wordToFrames->at(word.getId()).second;

	length_t framesCnt = lastId - firstId + 1;
	double* mfcc = new double[MFCC_SIZE * framesCnt];

	for (uint32_t i = 0; i < framesCnt; i++) {
		length_t rawBegin = this->frameToRaw->at(firstId + i).first;
		length_t rawFinsh = this->frameToRaw->at(firstId + i).second;

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
	length_t frameStart = -1;
	for (length_t currentFrame = this->wordToFrames->at(word.getId()).first;
			currentFrame < this->wordToFrames->at(word.getId()).second; currentFrame++) {
		frameStart = this->frameToRaw->at(currentFrame).first;

		for (length_t i = 0; i < samplesPerNonOverlap; i++) {
			data[frameNumber * samplesPerNonOverlap + i ] =
					this->wavData->getRawData()->at(frameStart + i);

			//DEBUG("Frame %d (%d): %d", frameNumber,
			//		frameStart + i, this->wavData->getRawData()->at(frameStart + i));
		}

		frameNumber++;
	}

	fs.write((char*)data, waveSize);
	fs.close();
	delete [] data;
}

bool Processor::isPartOfAWord(const Frame& frame) const {
	bool isPartOfWord = false;

	for (std::map<length_t, std::pair<length_t, length_t> >::const_iterator word = this->wordToFrames->begin();
			word != this->wordToFrames->end(); ++word) {

		if (word->second.first <= frame.getId() && frame.getId() <= word->second.second) {
			isPartOfWord = true;
			break;
		}
	}

	return isPartOfWord;
}

length_t Processor::getFramesCount(const Word& word) const {
	 length_t cnt = this->wordToFrames->at(word.getId()).second - this->wordToFrames->at(word.getId()).first;
	 return cnt;
}

} /* namespace audio */
} /* namespace wtm */
