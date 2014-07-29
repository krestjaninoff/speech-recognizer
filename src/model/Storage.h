#ifndef STORAGE_H_
#define STORAGE_H_

#include <HmModel.h>
#include <CodeBook.h>
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
 * Note, this implementation is quite ineffective. But this is better than nothing :)
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
	 * Get list of the available models
	 */
	const map<uint32_t, HmModel*>* getModels() { return this->models; }

	/**
	 * Add model
	 */
	uint32_t addModel(HmModel* model);

	/**
	 * Delete model
	 */
	void deleteModel(uint32_t id);


	/**
	 * Get codebook
	 */
	const CodeBook* getCodeBook() { return this->codeBook; }

	/**
	 * Add label (update if exists)
	 */
	void addLabel(observation_t label, MfccEntry* mfccEntry);

	/**
	 * Delete label
	 */
	void deleteLabel(observation_t label);


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
	 * List of models
	 */
	map<uint32_t, HmModel*>* models;

	/**
	 * Code book
	 */
	CodeBook* codeBook;

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
