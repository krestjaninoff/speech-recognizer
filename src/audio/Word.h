/*
 * Word is a set of frames that match a word from a dictionary
 */

#ifndef WORD_H_
#define WORD_H_

#include <vector>
#include "Frame.h"

namespace wtm {
namespace audio {

class Word {

public:
	Word(const std::vector<Frame*>* frames);

	void setText(const std::string& text) { this->text = text; }
	const std::string& getText() const { return text; };

	const std::vector<Frame*>& getFrames() const { return *frames; }

private:
	const std::vector<Frame*>* frames;
	std::string                 text;
};

} /* namespace audio */
} /* namespace wtm */

#endif /* WORD_H_ */
