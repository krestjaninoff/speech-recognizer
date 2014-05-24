#include <Storage.h>

namespace wtm {
namespace command {

	Storage::Storage() {
		this->maxId = 0;
		this->models = NULL;
	}

	Storage::~Storage() {

		if (NULL != this->models) {
			for (std::map<uint32_t, Model*>::const_iterator model = this->models->begin();
					model != this->models->end(); ++model) {
				delete *model;
			}

			delete this->models;
		}
	}

	bool Storage::init() {

		// Check if already initialized
		if (NULL != this->models) {
			return true;
		}
		this->models = new map<uint32_t, Model*>();

		if (-1 != access(STORAGE_FILE, F_OK)) {
			fprintf(stdout, "Loading models storage... ");

			std::fstream fs;
			fs.open(STORAGE_FILE, std::ios::in | std::ios::binary);

			if (!fs.good()) {
				fprintf(stderr, "Can't access models storage\n");
				return false;
			}

			char header[4];
			fs.read(header, sizeof(char) * 4);
			if (strncmp(header, STORAGE_HEADER, 4)) {
				fprintf(stderr, "Invalid storage\n");
				return false;
			}

			this->maxId << fs;

			for (uint32_t i = 0; i < this->maxId; i++) {
				Model* model;
				model << fs;

				this->models->insert(make_pair(model->getId(), model));
			}

			fs.close();
			fprintf(stdout, "done!\n");

		// Storage not found, creating an empty one
		} else {
			fprintf(stdout, "Storage not found, creating an empty one... ");

			std::fstream fs;
			fs.open(STORAGE_FILE, std::ios::out | std::ios::binary);
			fs.close();

			fprintf(stdout, "done!\n");
		}

		return true;
	}

	uint32_t Storage::addModel(const Model* model) {

		model->setId(++this->maxId);
		this->models->insert(make_pair(this->maxId, *model));

		return this->maxId;
	}

	void Storage::addSample(uint32_t modelId, const Word& word) {

		Model* model = this->models->at(modelId);
		model->addSample(word.getMfcc(), word.getMfccSize());
	}

	bool Storage::persist() {
		fprintf(stdout, "Storage not found, creating an empty one... ");

		std::fstream fs;
		fs.open(STORAGE_FILE, std::ios::out | std::ios::binary);

		if (!fs.good()) {
			fprintf(stderr, "Can't access models storage\n");
			return false;
		}

		fs.write(STORAGE_HEADER, sizeof(char) * 4);
		this->maxId >> fs;

		for (std::map<uint32_t, Model*>::const_iterator model = this->models->begin();
				model != this->models->end(); ++model) {

			*model >> fs;
		}

		fs.close();
		fprintf(stdout, "done!\n");
	}

} /* namespace command */
} /* namespace wtm */
