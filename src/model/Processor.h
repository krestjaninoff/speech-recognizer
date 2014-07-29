#ifndef RECOGNIZER_H_
#define RECOGNIZER_H_

#include <HmModel.h>
#include <Codebook.h>
#include <Word.h>
#include <vector>

using namespace std;
using namespace yazz::audio;

namespace yazz {
namespace model {

/**
 * Processor is designed to solve two main tasks:
 * 1. Train the models;
 * 2. Find the best model for an entry set of mfcc data;
 */
class Processor {
public:
	Processor(Storage& storage);
	~Processor();

	void trainModel(HmModel* model, vector<MfccEntry*>* data);
	HmModel* findBestModel(const vector<MfccEntry*>* data);

private:
	Storage& storage;

	const vector<observation_t>* mfccToLabels(const vector<MfccEntry*>* mfcc);
};

} /* namespace model */
} /* namespace yazz */

#endif /* RECOGNIZER_H_ */
