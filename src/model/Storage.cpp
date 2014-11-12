#include <string.h>
#include <Storage.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <utility>
#include <../config.h>

#ifdef __MINGW32__
#include <io.h>
#endif

using namespace std;

namespace yazz {
namespace model {

const char* STORAGE_FILE = "models.dat";

const string STORAGE_HEADER = "YAZZ";
const string MAX_ID = "MAX_ID";
const string MODELS = "MODELS";

// Common attributes
const string Storage::SPACE = " ";
const string Storage::TAB = "\t";
const int Storage::PRECISION = 5;
const int Storage::INVALID_CODE = 1;

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
		cout << "Loading data from the storage..." << endl;

		std::ifstream fs;
		fs.open(STORAGE_FILE, std::ios::in);

		if (!fs.good()) {
			cerr << "Can't access the data storage :(" << endl;
			exit(INVALID_CODE);
		}

		// Read the header
		readHeader(fs, STORAGE_HEADER);

		// Read MAX_ID section
		this->maxId = readNamedInt(fs, MAX_ID, true);

		// Read MODEL section
		int32_t modelsCnt = readNamedInt(fs, MODELS, true);

		// Read models
		for (int32_t i = 0; i < modelsCnt; i++) {
			HmModel* model = new HmModel();
			fs >> *model;

			this->models->insert(make_pair(model->getId(), model));

			if (DEBUG_ENABLED) {
				cout << ".. model " << model->getText() << endl;
			}
		}

		// Read the Codebook
		fs >> *(this->codeBook);

		fs.close();

	// Storage not found, creating an empty one
	} else {
		cout << "Storage not found, creating an empty one... " << endl;
		persist();
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

	std::ofstream fs;
	fs.open(STORAGE_FILE, std::ios::out);

	if (!fs.good()) {
		cerr << "Can't access the data storage :(" << endl;
		return false;
	}

	fs << STORAGE_HEADER << endl;
	fs << MAX_ID << SPACE << this->maxId << endl;
	fs << endl << endl;

	fs << MODELS << SPACE << this->models->size() << endl;
	fs << endl;

	map<uint32_t, HmModel*>::const_iterator iter;
	for (iter = this->models->begin(); iter != this->models->end(); ++iter) {
		HmModel& model = *iter->second;
		fs << model << endl;
	}
	fs << endl;

	fs << *(this->codeBook);

	fs.close();
	cout << "...storage data successfully updated..." << endl;

	return true;
}


void Storage::readHeader(istream& fs, string name) {
	string tmpStr;

	bool isOk = (fs >> tmpStr);
	processReadResults(isOk, name, tmpStr);
}

int Storage::readNamedInt(istream& fs, string name, bool isPositive) {
	int32_t result = 0;
	string tmpStr;

	bool isOk = (fs >> tmpStr >> result);
	processReadResults(isOk, name, tmpStr);

	if (isPositive && result < 0) {
		cerr << "Invalid storage: " << name << " should be a positive value" << endl;
		exit(INVALID_CODE);
	}

	return result;
}

double Storage::readNamedDouble(istream& fs, string name) {
	double result = 0;
	string tmpStr;

	bool isOk = (fs >> tmpStr >> result);
	processReadResults(isOk, name, tmpStr);

	return result;
}

string Storage::readNamedString(istream& fs, string name) {
	string result;
	string tmpStr;

	bool isOk = (fs >> tmpStr >> result);
	processReadResults(isOk, name, tmpStr);

	return result;
}

void Storage::processReadResults(bool isOk, string name, string actualValue) {

	if (isOk) {
		if (0 != name.compare(actualValue)) {
			cerr << "Invalid storage: " << name << " is missing" << endl;
			exit(INVALID_CODE);
		}

	} else {
		cerr << "Invalid storage: " << name << " is missing or corrupted" << endl;
		exit(INVALID_CODE);
	}
}

} /* namespace model */
} /* namespace yazz */
