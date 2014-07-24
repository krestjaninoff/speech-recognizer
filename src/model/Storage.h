#ifndef STORAGE_H_
#define STORAGE_H_

#include <SimpleModel.h>
#include <Word.h>
#include <cstdint>
#include <map>

using namespace std;
using namespace yazz::model;
using namespace yazz::audio;

namespace yazz {
namespace model {

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
	const map<uint32_t, SimpleModel*>* getModels() { return this->models; }

	/**
	 * Add model
	 */
	uint32_t addModel(SimpleModel* model);

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
	map<uint32_t, SimpleModel*>* models;

	/**
	 * Models data base
	 */
	static const char* STORAGE_FILE;

	/**
	 * Storage-specific header
	 */
	static const char* STORAGE_HEADER;
};


} /* namespace model */
} /* namespace yazz */

#endif /* STORAGE_H_ */
