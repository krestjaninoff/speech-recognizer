#include <CodeBook.h>

namespace yazz {
namespace model {

CodeBook::CodeBook() {
	this->book = new std::map<observation_t, CodeBookEntry*>();
}

CodeBook::~CodeBook() {
	if (NULL != this->book) {

		std::map<observation_t, CodeBookEntry*>::iterator iterator;
		for (iterator = this->book->begin(); iterator != this->book->end(); iterator++) {
			if (NULL != iterator->second) {
				delete iterator->second;
			}
		}

		delete this->book;
	}
}

void CodeBook::addLabeledSanple(observation_t label, MfccEntry& mfccEntry) {
	CodeBookEntry* entry;

	// It's the first label
	if (NULL == this->book[label]) {
		entry = new CodeBookEntry();
		entry->samplesCnt = 1;
		entry->avgVector = mfccEntry;

		this->book[label] = entry;

	// Not the first label
	} else {
		entry = this->book[label];
		double* avgVector = entry->avgVector->getData();
		double* currentVector = mfccEntry.getData();

		for (size_t i = 0; i < entry->avgVector->getSize(); i++) {
			double diff = (avgVector[i] - currentVector[i]) / static_cast<double>(entry->samplesCnt);
			avgVector[i] += diff;
		}

		entry->samplesCnt++;
	}
}

observation_t CodeBook::findLabelBySample(MfccEntry& mfccEntry) {
	double minDistance = -1;
	observation_t label = '\0';

	std::map<observation_t, CodeBookEntry*>::iterator iterator;
	for (iterator = this->book->begin(); iterator != this->book->end(); iterator++) {
		MfccEntry* currentMfccEntry = iterator->second->avgVector;

		double distance = math::Basic::euclideanDistance(mfccEntry.getData(),
				currentMfccEntry->getData(), MFCC_SIZE);

		if (fabs(distance) < CODEBOOK_THRESHOLD) {
			if ('\0' == label || minDistance > distance) {
				minDistance = distance;
				label = iterator->first;
			}
		}
	}

	return label;
}

ostream& operator<<(ostream& fs, const CodeBook& obj) {

	size_t bookSize = obj.book->size();
	fs.write((char*)(&bookSize), sizeof(size_t));

	std::map<observation_t, CodeBookEntry*>::iterator iterator;
	for (iterator = obj.book->begin(); iterator != obj.book->end(); iterator++) {
		fs.write((char*)(&iterator->first), sizeof(observation_t));
		fs.write((char*)(&iterator->second->samplesCnt), sizeof(uint16_t));
		fs << iterator->second->avgVector;
	}

	return fs;
}

istream& operator>>(istream& fs, CodeBook& obj) {
	obj.book->clear();

	size_t bookSize;
	fs.read((char*) &bookSize, sizeof(size_t));

	for (size_t i = 0; i < bookSize; i++) {
		CodeBookEntry* bookEntry = new CodeBookEntry();

		observation_t label;
		fs.read((char*) &label, sizeof(observation_t));
		fs.read((char*) &bookEntry->samplesCnt, sizeof(uint16_t));

		MfccEntry* mfccEntry = new MfccEntry();
		fs >> mfccEntry;

		bookEntry->avgVector = mfccEntry;
		obj.book[label] = bookEntry;
	}

	return fs;
}

} /* namespace model */
} /* namespace yazz */
