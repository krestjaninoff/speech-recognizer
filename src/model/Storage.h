#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include "Model.h"
#include "../audio/Word.h"

using namespace std;
using namespace yazz::math;
using namespace yazz::audio;

namespace yazz {
namespace command {

/**
 * Just a simple binary storage
 * <p>
 * Its implementation is quite ineffective. But this is better than nothing :)
 */
class Storage {

public:

	Storage();
	~Storage();

	/**
	 * Initialization
	 */
	bool init();

	/**
	 * Load models from its storage
	 */
	const map<uint32_t, Model*>* getModels() { return this->models; }

	/**
	 * Add model
	 */
	uint32_t addModel(Model* model);

	/**
	 * Add the sample into the model
	 */
	void addSample(uint32_t modelId, const Word& word);

	/**
	 * Flush data into FS
	 */
	bool persist();

private:

	/**
	 * Max model's ID
	 */
	uint32_t maxId;

	/**
	 * Models
	 */
	map<uint32_t, Model*>* models;

	/**
	 * Models data base
	 */
	static const char* STORAGE_FILE;

	/**
	 * Storage-specific header
	 */
	static const char* STORAGE_HEADER;
};


} /* namespace command */
} /* namespace yazz */

#endif /* STORAGE_H_ */
