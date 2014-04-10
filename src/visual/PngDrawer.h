/*
 * A simple instrument for data visualization
 */
#ifndef PNG_DRAWER_H_
#define PNG_DRAWER_H_

#include <string>
#include "../audio/audio.h"
#include "../audio/WavData.h"

namespace wtm {
namespace visual {

/**
 * Draw PNG visualization for the data
 * <p>
 * Uses libpng library.
 * @see http://www.libpng.org/pub/png/book/
 */
class PngDrawer {

public:
	static void drawToFile(const audio::WavDataPtr wav_data, const std::string& file);

private:
	static int writeImage(const char* filename, bool* buffer, uint32_t width,
			uint32_t height);

}; // class PngDrawer

} // namespace visual
} // namespace wtm

#endif /* PNG_DRAWER_H_ */
