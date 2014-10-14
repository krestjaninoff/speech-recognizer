#include <stdlib.h>
#include <Storage.h>
#include <MfccEntry.h>
#include <HmModel.h>

namespace yazz {
namespace model {

MfccEntry::MfccEntry() {
	this->size = 0;
	this->data = NULL;
}

MfccEntry::MfccEntry(double* data, size_t size) {
	this->size = size;
	this->data = data;
}

MfccEntry::~MfccEntry() {
	if (NULL != this->data) {
		delete [] this->data;
	}
}

ostream& operator<<(ostream& fs, const MfccEntry& obj) {
	streamsize precisionOriginal = fs.precision(Storage::PRECISION);

	fs << obj.size << Storage::SPACE;
	for (size_t i = 0; i < obj.size; i++) {
		fs << obj.data[i] << Storage::SPACE;
	}

	fs.precision(precisionOriginal);
	return fs;
}

istream& operator>>(istream& fs, MfccEntry& obj) {

	if (fs >> obj.size) {
		obj.data = new double[obj.size];

		for (size_t i = 0; i < obj.size; i++) {
			if (!(fs >> obj.data[i])) {

				cerr << "Invalid model: INITIAL data corrupted" << endl;
				exit(Storage::INVALID_CODE);
			}
		}
	} else {
		cerr << "Invalid MFCC: size isn't specified" << endl;
		exit(Storage::INVALID_CODE);
	}

	return fs;
}

void MfccEntry::print() {
	cout << "[";

	for (size_t i = 0; i < this->size; i++) {
		cout << this->data[i];

		if (i < this->size - 1) {
			cout << ", ";
		}
	}

	cout << "]" << endl;
}

} /* namespace model */
} /* namespace yazz */
