#include <Word.h>

namespace wtm {
namespace audio {

	Word::Word(const std::vector<Frame*>* frames):
		frames(frames), text(0) {
	}

} /* namespace audio */
} /* namespace wtm */
