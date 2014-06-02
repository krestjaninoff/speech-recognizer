#ifndef PAINTER_H_
#define PAINTER_H_

#include "../audio/Processor.h"
#include <cstdint>
#include <string>

namespace yazz {
namespace visual {

/**
 * Simple instrument for data visualization
 * <p>
 * Uses libpng library.
 * @see http://www.libpng.org/pub/png/book/
 */
class Painter {

public:
	static void drawRawData(const audio::Processor* Processor, const std::string& file);
	static void drawFrames(const audio::Processor* Processor, const std::string& file);

private:
	static int writeImage(const char* filename, uint8_t* buffer, uint32_t width,
			uint32_t height);

}; // class Painter

} // namespace visual
} // namespace yazz

#endif /* PAINTER_H_ */
