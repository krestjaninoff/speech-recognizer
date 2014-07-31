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

	fs.write((char*)(&obj.size), sizeof(size_t));
	fs.write(reinterpret_cast<char*>(obj.data),
			streamsize(obj.size * sizeof(double)));

	return fs;
}

istream& operator>>(istream& fs, MfccEntry& obj) {

	fs.read((char*)(&obj.size), sizeof(size_t));
	obj.data = new double[obj.size];
	fs.read(reinterpret_cast<char*>(obj.data),
			streamsize(obj.size * sizeof(double)));

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
