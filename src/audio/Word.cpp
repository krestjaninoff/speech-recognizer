#include <Word.h>

namespace wtm {
namespace audio {

	Word::Word(const std::vector<Frame*>* frames) {
		this->frames = frames;
		this->text = "";
	}

} /* namespace audio */
} /* namespace wtm */
