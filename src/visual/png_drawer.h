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
 * Uses png++/libpng library.
 * @see http://savannah.nongnu.org/projects/pngpp/
 */
void draw_png(audio::wav_data_t* wav_data, std::string file);

} // namespace wtm
} // namespace visual

#endif /* PNG_DRAWER_H_ */
