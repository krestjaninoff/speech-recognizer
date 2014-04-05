/*
 * A simple instrument for data visualization
 */
#ifndef PNG_DRAWER_H_
#define PNG_DRAWER_H_

#include <string>
#include "../audio/audio.h"

namespace wtm {
namespace visual {

/**
 * Draw PNG visualization for the data
 * <p>
 * Uses libpng library.
 * @see http://www.libpng.org/pub/png/book/
 */
void draw_png(audio::wav_data_t* wav_data, std::string file);

} // namespace wtm
} // namespace visual

#endif /* PNG_DRAWER_H_ */
