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
#include <Splitter.h>

using namespace std;

namespace wtm {
namespace audio {

void Splitter::split() {

	// Init "samples per frame" measure
	length_t bytesPerFrame = static_cast<length_t>(
			getWavData()->getHeader().bytesPerSec * FRAME_LENGTH / 1000.0);
	length_t bytesPerSample = static_cast<uint32_t>(
			getWavData()->getHeader().bitsPerSample / 8);
	samplesPerFrame = static_cast<length_t>(bytesPerFrame / bytesPerSample);
	assert(samplesPerFrame > 0);

	// The main part of splitting
	divideIntoFrames();
	divideIntoWords();
}

void Splitter::divideIntoFrames() {

	unsigned int samplesPerNonOverlap =
		static_cast<unsigned int>(samplesPerFrame * (1 - FRAME_OVERLAP));
	unsigned int framesCount =
		(getWavData()->getHeader().subchunk2Size / (getWavData()->getHeader().bitsPerSample / 8))
			/ samplesPerNonOverlap;

	frames->reserve(framesCount);

	length_t indexBegin = 0, indexEnd = 0;
	for (length_t frameId = 0, size = getWavData()->getRawData()->size(); frameId < framesCount;
			++frameId) {

		indexBegin = frameId * samplesPerNonOverlap;
		indexEnd = indexBegin + samplesPerFrame;
		if (indexEnd < size) {

			Frame* frame = new Frame(frameId);
			frame->init(*getWavData()->getRawData(), indexBegin, indexEnd);
			frames->push_back(frame);
			frameToRaw->insert(std::make_pair(frameId, make_pair(indexBegin, indexEnd)));
		} else {
			break;
		}
	}
}

void Splitter::divideIntoWords() {
	assert(frames->size() > 10);

	double maMin = 0;
	double maAvg = 0;
	double maMax = 0;
	double ma;

	// Let's use Moving Average value to avoid spikes
	unsigned short maShift = MOVING_AVERAGE_SIZE / 2;
	maAvg = maMin = frames->at(0)->getRms();
	length_t iFrame;
	for (iFrame = maShift; iFrame < frames->size() - maShift; ++iFrame) {

		ma = 0;
		for (unsigned short iMa = iFrame - maShift; iMa <= iFrame + maShift; iMa++) {
			ma += frames->at(iMa)->getRms();
		}
		ma /= MOVING_AVERAGE_SIZE;
		frames->at(iFrame)->setMaRms(ma);

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
		frames->at(iFrame)->setMaRms(frames->at(iFrame)->getRms());
		frames->at(frames->size() - 1 - iFrame)->setMaRms(
				frames->at(frames->size() - 1 - iFrame)->getRms());
	}

	// Tries to guess the best threshold value
	double thresholdCandidate = getThresholdCandidate(maMin, maAvg, maMax);
	this->wordsThreshold = thresholdCandidate;

	// If max value greater than min value more then 50% then we have the "silence" threshold.
	// Otherwise, let's think that we have only one word.
	double threshold = 0;
	if (maMax * 0.5 > maMin) {
		threshold = thresholdCandidate;

		// Divide frames into words
		length_t wordId = 0;
		long firstFrameInCurrentWordNumber = -1;
		vector<Frame*>::const_iterator firstFrame;
		vector<Frame*>::const_iterator lastFrame;
		Word* lastWord = 0;
		for (vector<Frame*>::const_iterator frame = frames->begin();
				frame != frames->end(); ++frame) {

			// Got a sound
			if ((*frame)->getMaRms() > threshold) {

				if (-1 == firstFrameInCurrentWordNumber) {
					firstFrameInCurrentWordNumber = (*frame)->getId();
					DEBUG("Word started at frame %d", firstFrameInCurrentWordNumber);
				}

			// Got silence
			} else {
				if (firstFrameInCurrentWordNumber >= 0) {

					// Let's find distance between start of the current word and end of the previous word
					length_t distance = 0;
					if (0 != lastWord) {
						Frame* lastFrameInPreviousWord = lastWord->getFrames()->at(
								lastWord->getFrames()->size() - 1);
						distance = firstFrameInCurrentWordNumber - lastFrameInPreviousWord->getId();
					}

					// We have a new word
					if (0 == lastWord || distance >= WORDS_MIN_DISTANCE) {
						lastWord = new Word(wordId++);

						wordToFrames->insert(make_pair(wordId,
								make_pair(firstFrameInCurrentWordNumber, (*frame)->getId())));
						this->words->push_back(lastWord);

						DEBUG("Word finished at frame %d", (*frame)->getId());

					// We need to add the current word to the previous one
					} else if (0 != lastWord && distance < WORDS_MIN_DISTANCE) {
						lastWord = new Word(wordId);

						this->words->pop_back();
						this->words->push_back(lastWord);
						wordToFrames->insert(make_pair(wordId,
								make_pair((*lastWord->getFrames()->begin())->getId(), (*frame)->getId())));

						DEBUG("Word finished at frame %d and added to previous one", frame - frames->begin());
					}

					firstFrameInCurrentWordNumber = -1;
				}
			}
		}

		// Clean up short words
		for (vector<Word*>::iterator word = this->words->begin();
				word != this->words->end(); ++word) {
			if ((*word)->getFramesCount() < WORD_MIN_SIZE) {
				this->words->erase(word);
			}
		}


	// Seems we have only one word
	} else {
		this->words->push_back(new Word(this->frames));
	}
}

/**
 * Determination of silence threshold
 *
 * Method divides data into 3 clusters (using something like k-means algorithm).
 * The cluster center of "Min" cluster is used as a threshold candidate.
 */
double Splitter::getThresholdCandidate(double maMin, double maAvg, double maMax) {
	short currIter = 0, maxIterCnt = 30;
	bool isCenterChanged = true;

	// Init clusters
	double minClusterCenter = maMin;
	double minClusterCenterNew = 0;
	std::vector<Frame*>* minCluster = new std::vector<Frame*>();

	// TODO May be maAvg will serve better?
	double avgClusterCenter = maMax / 2;
	double avgClusterCenterNew = 0;
	std::vector<Frame*>* avgCluster = new std::vector<Frame*>();

	double maxClusterCenter = maMax;
	double maxClusterCenterNew = 0;
	std::vector<Frame*>* maxCluster = new std::vector<Frame*>();

	double maRms;
	for (vector<Frame*>::const_iterator frame = frames->begin();
		frame != frames->end(); ++frame) {

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

		DEBUG("Min center: %f, size: %d", minClusterCenter, minCluster->size());
		DEBUG("Avg center: %f, size: %d", avgClusterCenter, avgCluster->size());
		DEBUG("Max center: %f, size: %d", maxClusterCenter, maxCluster->size());
		DEBUG("_");

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

		for (vector<Frame*>::const_iterator frame = frames->begin();
				frame != frames->end(); ++frame) {

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

void Splitter::saveToFile(const std::string& file, const Word& word) const {

	// number of data bytes in the resulting wave file
	unsigned int samplesPerNonOverlap =
			static_cast<unsigned int>(samplesPerFrame * (1 - FRAME_OVERLAP));
	unsigned int waveSize = word.getFrames()->size() * samplesPerNonOverlap * sizeof(raw_t);

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
	for (vector<Frame*>::const_iterator frame = word.getFrames()->begin();
			frame != word.getFrames()->end(); ++frame) {

		for (length_t i = 0; i < samplesPerNonOverlap; i++) {
			data[frameNumber * samplesPerNonOverlap + i ] =
					this->rawData->at((*frame)->getStart() + i);

			DEBUG("Frame %d (%d): %d", frameNumber,
					(*frame)->getStart() + i, this->rawData->at((*frame)->getStart() + i));
		}

		frameNumber++;
	}

	fs.write((char*)data, waveSize);
	fs.close();
	delete [] data;
}

bool Splitter::isPartOfAWord(const Frame* frame) const {
	bool isPartOfWord = false;

	for (vector<Word*>::const_iterator word = this->words->begin();
			word != this->words->end(); ++word) {

		length_t left = (*(*word)->getFrames()->begin())->getId();
		length_t right = (*word)->getFrames()->at((*word)->getFrames()->size() - 1)->getId();

		if (left <= frame->getNumber() && frame->getNumber() <= right) {

			isPartOfWord = true;
			break;
		}
	}

	return isPartOfWord;
}

length_t Splitter::getFramesCount() const {
	return std::distance(this->frames->begin(), this->frames->end());
}

} /* namespace audio */
} /* namespace wtm */
