#include <string.h>
#include <Storage.h>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#ifdef __MINGW32__
#include <io.h>
#endif

namespace yazz {
namespace model {

const char* Storage::STORAGE_FILE = "hmm.dat";
const char* Storage::STORAGE_HEADER = "YAZZ";

Storage::Storage() {
	this->maxId = 0;
	this->models = NULL;
	this->codeBook = NULL;
}

Storage::~Storage() {

	if (NULL != this->models) {
		map<uint32_t, HmModel*>::const_iterator iter;
		for (iter = this->models->begin(); iter != this->models->end(); ++iter) {
			delete iter->second;
		}

		delete this->models;
	}

	if (NULL != this->codeBook) {
		delete codeBook;
	}
}

bool Storage::init() {

	// Check if already initialised
	if (NULL != this->models && NULL != this->codeBook) {
		return true;
	}

	this->models = new map<uint32_t, HmModel*>();
	this->codeBook = new CodeBook();

	if (access(STORAGE_FILE, F_OK) != -1) {
		cout << "...loading data from the storage..." << endl;

		std::fstream fs;
		fs.open(STORAGE_FILE, std::ios::in | std::ios::binary);

		if (!fs.good()) {
			cerr << "Can't access the data storage :(" << endl;
			return false;
		}

		char header[4];
		fs.read(header, sizeof(char) * 4);
		if (strncmp(header, STORAGE_HEADER, 4)) {
			cerr << "Invalid storage :(" << endl;
			return false;
		}

		fs.read((char*) &this->maxId, sizeof(uint32_t));

		size_t modelsCnt;
		fs.read((char*) &modelsCnt, sizeof(uint32_t));

		for (uint32_t i = 0; i < modelsCnt; i++) {
			HmModel* model = new HmModel();
			fs >> *model;

			this->models->insert(make_pair(model->getId(), model));
		}

		fs >> *(this->codeBook);

		fs.close();

	// Storage not found, creating an empty one
	} else {
		cout << "Storage not found, creating an empty one... " << endl;

		std::fstream fs;
		fs.open(STORAGE_FILE, std::ios::out | std::ios::binary);

		fs.write(STORAGE_HEADER, sizeof(char) * 4);
		fs.write((char*) &this->maxId, sizeof(uint32_t));

		size_t modelsCnt = 0;
		fs.write((char*) &modelsCnt, sizeof(uint32_t));

		fs.close();
	}

	return true;
}

uint32_t Storage::addModel(HmModel* model) {

	model->setId(++this->maxId);
	this->models->insert(make_pair(this->maxId, model));

	return this->maxId;
}

void Storage::deleteModel(uint32_t id) {
	this->models->erase(id);
}

void Storage::addLabel(observation_t label, MfccEntry* mfccEntry) {
	return this->codeBook->addLabel(label, mfccEntry);
}

void Storage::deleteLabel(observation_t label) {
	this->codeBook->removeLabel(label);
}


/**
 * Save models into the file
 * <p>
 * We shouldn't rewrite the whole file. This is a point to fix.
 */
bool Storage::persist() {

	std::fstream fs;
	fs.open(STORAGE_FILE, std::ios::out | std::ios::binary);

	if (!fs.good()) {
		cerr << "Can't access the data storage :(" << endl;
		return false;
	}

	fs.write(STORAGE_HEADER, sizeof(char) * 4);
	fs.write((char*) &this->maxId, sizeof(uint32_t));

	size_t modelsCnt = this->models->size();
	fs.write((char*) &modelsCnt, sizeof(uint32_t));

	map<uint32_t, HmModel*>::const_iterator iter;
	for (iter = this->models->begin(); iter != this->models->end(); ++iter) {
		HmModel& model = *iter->second;
		fs << model;
	}

	fs << *(this->codeBook);

	fs.close();
	cout << "...storage data successfully updated..." << endl;

	return true;
}

} /* namespace model */
} /* namespace yazz */
