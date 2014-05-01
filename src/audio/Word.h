#ifndef WORD_H_
#define WORD_H_

#include <vector>
#include "Frame.h"

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

	/**
	 * Text meaning of the word
	 */
	const std::string& getText() const { return text; };
	void setText(const std::string& text) { this->text = text; }


private:
	length_t id;
	std::string text;
};

} /* namespace audio */
} /* namespace wtm */

#endif /* WORD_H_ */
