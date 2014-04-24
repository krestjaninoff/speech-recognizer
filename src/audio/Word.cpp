#include <Word.h>

namespace wtm {
namespace audio {

	Word::Word(const std::vector<Frame*>* frames) {
		this->frames = frames;
		this->text = "";
	}

	length_t Word::getFramesCount() const {
		return std::distance(this->frames->begin(), this->frames->end());
	}

} /* namespace audio */
} /* namespace wtm */
