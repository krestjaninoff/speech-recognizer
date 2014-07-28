#ifndef CODEBOOK_H_
#define CODEBOOK_H_

#include <MfccEntry.h>

using namespace std;

namespace yazz {
namespace model {

/**
 * Vector template for a special label
 */
struct CodeBookEntry {
	MfccEntry*	avgVector;
	uint16_t	samplesCnt;
};

/**
 * CodeBook
 *
 * The codebook is used for vector quantization process. It contains a set of
 * spectral vectors (mfcc) templates and related output labels. Using the codebook
 * you can convert a sequence of spectral vectors from input data into a sequence of
 * best template labels (discrete observations).
 */
class CodeBook {
public:
	CodeBook();
	~CodeBook();

	void addLabeledSanple(observation_t label, MfccEntry& mfccEntry);
	observation_t findLabelBySample(MfccEntry& mfccEntry);

private:
	map<observation_t, CodeBookEntry*>* book;
};

/**
 * Default value for unrecognised mfcc samples
 */
static char CODEBOOK_UNKNOWN_VALUE = '?';

} /* namespace model */
} /* namespace yazz */

#endif /* CODEBOOK_H_ */
