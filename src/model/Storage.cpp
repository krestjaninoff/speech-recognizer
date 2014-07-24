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

	const char* Storage::STORAGE_FILE = "models.dat";
	const char* Storage::STORAGE_HEADER = "YAZZ";

	Storage::Storage() {
		this->maxId = 0;
		this->models = NULL;
	}

	Storage::~Storage() {

		if (NULL != this->models) {
			for (std::map<uint32_t, SimpleModel*>::const_iterator model = this->models->begin();
					model != this->models->end(); ++model) {
				delete (*model).second;
			}

			delete this->models;
		}
	}

	bool Storage::init() {

		// Check if already initialized
		if (NULL != this->models) {
			return true;
		}
		this->models = new map<uint32_t, SimpleModel*>();

		if (access(STORAGE_FILE, F_OK) != -1) {
			cout << "Loading models from the storage... " << endl;

			std::fstream fs;
			fs.open(STORAGE_FILE, std::ios::in | std::ios::binary);

			if (!fs.good()) {
				cerr << "Can't access the model's storage :(" << endl;
				return false;
			}

			char header[4];
			fs.read(header, sizeof(char) * 4);
			if (strncmp(header, STORAGE_HEADER, 4)) {
				cerr << "Invalid storage :(" << endl;
				return false;
			}

			fs.read((char*) &this->maxId, sizeof(uint32_t));

			string tmpName("");
			for (uint32_t i = 0; i < this->maxId; i++) {
				SimpleModel* model = new SimpleModel(tmpName);
				fs >> *model;

				this->models->insert(make_pair(model->getId(), model));
			}

			fs.close();

		// Storage not found, creating an empty one
		} else {
			cout << "Storage not found, creating an empty one... " << endl;

			std::fstream fs;
			fs.open(STORAGE_FILE, std::ios::out | std::ios::binary);

			fs.write(STORAGE_HEADER, sizeof(char) * 4);
			fs.write((char*) &this->maxId, sizeof(uint32_t));

			fs.close();
		}

		return true;
	}

	uint32_t Storage::addModel(SimpleModel* model) {

		model->setId(++this->maxId);
		this->models->insert(make_pair(this->maxId, model));

		return this->maxId;
	}

	void Storage::addSample(uint32_t modelId, const Word& word) {

		SimpleModel* model = (*this->models)[modelId];
		model->addSample(word.getMfcc(), word.getMfccSize());
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
			cerr << "Can't access model's storage :(" << endl;
			return false;
		}

		fs.write(STORAGE_HEADER, sizeof(char) * 4);
		fs.write((char*) &this->maxId, sizeof(uint32_t));

		for (std::map<uint32_t, SimpleModel*>::const_iterator model = this->models->begin();
				model != this->models->end(); ++model) {

			const SimpleModel& tmpModel = *((*model).second);
			fs << tmpModel;
		}

		fs.close();
		cout << "done!" << endl;

		return true;
	}

} /* namespace model */
} /* namespace yazz */
