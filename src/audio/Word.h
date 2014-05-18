#ifndef WORD_H_
#define WORD_H_

#include "audio.h"

namespace wtm {
namespace audio {

/**
 * Word is a set of frames that match a word from a dictionary
 */
class Word {

public:
	/**
	 * Create a word based on set of frames
	 */
	Word(length_t id);
	~Word();

	/**
	 * Get frame's serial number
	 */
	length_t getId() const { return this->id; }

	/**
	 * Text meaning of the word
	 */
	void setText(const std::string& text) { this->text = text; }
	const std::string& getText() const { return text; };

	void setMfcc(double* mfcc, unsigned int size) {
		this->mfcc = mfcc; this->mfccSize = size; }
	double* getMfcc() { return this->mfcc; }
	unsigned int getMfccSize() { return this->mfccSize; }

private:
	length_t id;
	std::string text;

	unsigned int mfccSize;
	double* mfcc;
};

} /* namespace audio */
} /* namespace wtm */

#endif /* WORD_H_ */
