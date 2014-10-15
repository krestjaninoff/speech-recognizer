#include "../config.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Storage.h>
#include <CodeBook.h>
#include <Basic.h>

namespace yazz {
namespace model {

const string CodeBook::UNKNOWN_VALUE = "?";
const string CODEBOOK = "CODEBOOK";

CodeBook::CodeBook() {
	this->book = new std::map<observation_t, CodeBookEntry*>();
}

CodeBook::~CodeBook() {
	if (NULL != this->book) {

		std::map<observation_t, CodeBookEntry*>::iterator iter;
		for (iter = this->book->begin(); iter != this->book->end(); iter++) {
			if (NULL != iter->second) {
				delete iter->second;
			}
		}

		delete this->book;
	}
}

void CodeBook::addLabel(observation_t label, MfccEntry* mfccEntry) {
	CodeBookEntry* entry;

	// It's the first label
	if (NULL == (*this->book)[label]) {
		entry = new CodeBookEntry();
		entry->samplesCnt = 1;
		entry->avgVector = mfccEntry;

		(*this->book)[label] = entry;

	// Not the first label
	} else {
		entry = (*this->book)[label];
		double* avgVector = entry->avgVector->getData();
		double* currentVector = mfccEntry->getData();

		for (size_t i = 0; i < entry->avgVector->getSize(); i++) {
			double correctedValue = (avgVector[i] * entry->samplesCnt + currentVector[i]) / (entry->samplesCnt + 1);
			avgVector[i] = correctedValue;
		}

		entry->samplesCnt++;
	}
}

void CodeBook::removeLabel(observation_t label) {
	this->book->erase(label);
}

observation_t CodeBook::findLabelBySample(MfccEntry* mfccEntry) const {
	double minDistance = -1;
	observation_t label = UNKNOWN_VALUE;

	std::map<observation_t, CodeBookEntry*>::iterator iterator;
	for (iterator = this->book->begin(); iterator != this->book->end(); iterator++) {
		MfccEntry* currentMfccEntry = iterator->second->avgVector;

		double distance = math::Basic::euclideanDistance(mfccEntry->getData(),
				currentMfccEntry->getData(), MFCC_SIZE);

		if (fabs(distance) < CODEBOOK_THRESHOLD) {
			if (0 == UNKNOWN_VALUE.compare(label) || minDistance > distance) {
				minDistance = distance;
				label = iterator->first;
			}
		}
	}

	return label;
}

ostream& operator<<(ostream& fs, const CodeBook& obj) {
	streamsize precisionOriginal = fs.precision(Storage::PRECISION);

	fs << CODEBOOK << Storage::SPACE <<  obj.book->size() << endl;

	std::map<observation_t, CodeBookEntry*>::iterator iterator;
	for (iterator = obj.book->begin(); iterator != obj.book->end(); iterator++) {

		fs << iterator->first << Storage::TAB << iterator->second->samplesCnt << Storage::TAB;
		fs << *iterator->second->avgVector;

		fs << endl;
	}

	fs.precision(precisionOriginal);
	return fs;
}

istream& operator>>(istream& fs, CodeBook& obj) {
	obj.book->clear();

	size_t bookSize = Storage::readNamedInt(fs, CODEBOOK, true);

	for (size_t i = 0; i < bookSize; i++) {
		CodeBookEntry* bookEntry = new CodeBookEntry();

		observation_t label;
		if (!(fs >> label)) {
			cerr << "Invalid CodeBook: label should be a string" << endl;
			exit(Storage::INVALID_CODE);
		}

		if (!(fs >> bookEntry->samplesCnt)) {
			cerr << "Invalid CodeBook: samples count must be a positive string" << endl;
			exit(Storage::INVALID_CODE);
		}

		MfccEntry* mfccEntry = new MfccEntry();
		fs >> *mfccEntry;

		bookEntry->avgVector = mfccEntry;
		(*obj.book)[label] = bookEntry;
	}

	return fs;
}

} /* namespace model */
} /* namespace yazz */
