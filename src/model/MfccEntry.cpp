#include <MfccEntry.h>

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
			streamsize(obj.size * sizeof(state_t)));

	return fs;
}

istream& operator>>(istream& fs, MfccEntry& obj) {

	fs.read((char*)(&obj.size), sizeof(size_t));
	obj.data = new state_t[obj.size];
	fs.read(reinterpret_cast<char*>(obj.data),
			streamsize(obj.size * sizeof(state_t)));

	return fs;
}

} /* namespace model */
} /* namespace yazz */
