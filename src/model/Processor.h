#ifndef RECOGNIZER_H_
#define RECOGNIZER_H_

#include <HmModel.h>
#include <CodeBook.h>
#include <Storage.h>
#include <Word.h>
#include <vector>

using namespace std;

namespace yazz {
namespace model {

/**
 * Processor is designed to solve two main tasks:
 * 1. Train the models;
 * 2. Find the best model for an entry set of mfcc data;
 */
class Processor {
public:
	Processor(Storage* storage);
	~Processor();

    const vector<observation_t>* mfccToObservations(const vector<MfccEntry*>* mfcc);

	void trainModel(HmModel* model, const vector<MfccEntry*>* data);
	const HmModel* findBestModel(const vector<HmModel*>* models, const vector<MfccEntry*>* data);

private:
	Storage* storage;
};

} /* namespace model */
} /* namespace yazz */

#endif /* RECOGNIZER_H_ */
