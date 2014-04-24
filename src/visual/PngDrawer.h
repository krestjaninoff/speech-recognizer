#ifndef PNG_DRAWER_H_
#define PNG_DRAWER_H_

#include <string>
#include "../common.h"
#include "../audio/audio.h"
#include "../audio/WavData.h"

namespace wtm {
namespace visual {

/**
 * Simple instrument for data visualization
 * <p>
 * Uses libpng library.
 * @see http://www.libpng.org/pub/png/book/
 */
class PngDrawer {

public:
	static void drawRawData(const audio::WavDataPtr wav_data, const std::string& file);
	static void drawFrames(const audio::WavDataPtr wav_data, const std::string& file);

private:
	static int writeImage(const char* filename, bool* buffer, uint32_t width,
			uint32_t height);

}; // class PngDrawer

} // namespace visual
} // namespace wtm

#endif /* PNG_DRAWER_H_ */
