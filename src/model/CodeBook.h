#ifndef CODEBOOK_H_
#define CODEBOOK_H_

#include <map>
#include <MfccEntry.h>
#include <HmModel.h>

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

	const map<observation_t, CodeBookEntry*>* getBook() const { return this->book; }

	void addLabel(observation_t label, MfccEntry* mfccEntry);
	void removeLabel(observation_t label);

	observation_t findLabelBySample(MfccEntry* mfccEntry) const;

	friend std::ostream& operator<<(std::ostream& fs, const CodeBook& obj);
	friend std::istream& operator>>(std::istream& fs, CodeBook& obj);

	/**
	 * Default value for unrecognised MFCC samples
	 */
	static const string UNKNOWN_VALUE;

private:
	map<observation_t, CodeBookEntry*>* book;
};


} /* namespace model */
} /* namespace yazz */

#endif /* CODEBOOK_H_ */
