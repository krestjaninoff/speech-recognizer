#include "../config.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Storage.h>
#include <CodeBook.h>
#include <Basic.h>

using namespace std;

namespace yazz {
namespace model {

const string CodeBook::UNKNOWN_VALUE = "?";
const string CODEBOOK = "CODEBOOK";
const double MFCC_WEIGHTS[] = {1.2, 1.1, 1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};


CodeBookEntry::CodeBookEntry(vector<MfccEntry*>* values) {
	this->values = values;
}

CodeBookEntry::~CodeBookEntry() {
	if (NULL != this->values) {
		vector<MfccEntry*>::iterator iter;
		for (iter = this->values->begin(); iter != this->values->end(); iter++) {
			delete *iter;
		}
	}
}
void CodeBookEntry::print() const {
	vector<MfccEntry*>::const_iterator iter;
	for (iter = this->values->begin(); iter != this->values->end(); iter++) {
		(*iter)->print();
	}
}

CodeBook::CodeBook() {
	this->book = new std::map<observation_t, CodeBookEntry*>();
}

CodeBook::~CodeBook() {
	if (NULL != this->book) {

		map<observation_t, CodeBookEntry*>::iterator iter;
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

		vector<MfccEntry*>* examples = new vector<MfccEntry*>();
		examples->push_back(mfccEntry);

		entry = new CodeBookEntry(examples);
		(*this->book)[label] = entry;

	// Not the first label
	} else {
		entry = (*this->book)[label];
		entry->addValue(mfccEntry);
	}
}

void CodeBook::removeLabel(observation_t label) {
	this->book->erase(label);
}

observation_t CodeBook::findLabelBySample(MfccEntry* mfccEntry) const {
	double minDistance = -1;
	observation_t label = UNKNOWN_VALUE;

	// Keep in mind that the first values in a MFCC vector are more meaningful than
	// the last ones. That's why we use the MFCC_WEIGHTS.

	std::map<observation_t, CodeBookEntry*>::const_iterator observationIter;
	for (observationIter = this->book->begin(); observationIter != this->book->end(); observationIter++) {

		std::vector<MfccEntry*>::const_iterator mfccIter;
		for (mfccIter = observationIter->second->getValues()->begin();
				mfccIter != observationIter->second->getValues()->end(); mfccIter++) {

			MfccEntry* currentMfccEntry = *mfccIter;

			double distance = math::Basic::euclideanDistanceWithWeights(mfccEntry->getData(),
					currentMfccEntry->getData(), MFCC_WEIGHTS, MFCC_SIZE);

			if (fabs(distance) < CODEBOOK_THRESHOLD) {
				if (0 == UNKNOWN_VALUE.compare(label) || minDistance > distance) {
					minDistance = distance;
					label = observationIter->first;
				}
			}
		}
	}

	return label;
}

ostream& operator<<(ostream& fs, const CodeBook& obj) {
	streamsize precisionOriginal = fs.precision(Storage::PRECISION);

	fs << CODEBOOK << Storage::SPACE <<  obj.book->size() << endl;

	std::map<observation_t, CodeBookEntry*>::iterator cbEntryIter;
	for (cbEntryIter = obj.book->begin(); cbEntryIter != obj.book->end(); cbEntryIter++) {

		fs << cbEntryIter->first << Storage::TAB << cbEntryIter->second->getSize() << endl;

		vector<MfccEntry*>::const_iterator mfccIter;
		for (mfccIter = cbEntryIter->second->getValues()->begin();
				mfccIter != cbEntryIter->second->getValues()->end(); mfccIter++) {
			fs << *mfccIter;
		}

		fs << endl;
	}

	fs.precision(precisionOriginal);
	return fs;
}

istream& operator>>(istream& fs, CodeBook& obj) {
	obj.book->clear();

	size_t bookSize = Storage::readNamedInt(fs, CODEBOOK, true);

	for (size_t i = 0; i < bookSize; i++) {

		observation_t label;
		if (!(fs >> label)) {
			cerr << "Invalid CodeBook: label should be a string" << endl;
			exit(Storage::INVALID_CODE);
		}
		if (DEBUG_ENABLED) {
			cout << ".. codebook entry " << label << endl;
		}

		size_t examplesCnt;
		if (!(fs >> examplesCnt)) {
			cerr << "Invalid CodeBook: examples count must be a positive string" << endl;
			exit(Storage::INVALID_CODE);
		}

		vector<MfccEntry*>* examples = new vector<MfccEntry*>();
		for (size_t j = 0; j < examplesCnt; j++) {

			MfccEntry* mfccEntry = new MfccEntry();
			fs >> *mfccEntry;

			examples->push_back(mfccEntry);
		}

		CodeBookEntry* bookEntry = new CodeBookEntry(examples);
		(*obj.book)[label] = bookEntry;
	}

	return fs;
}

} /* namespace model */
} /* namespace yazz */
