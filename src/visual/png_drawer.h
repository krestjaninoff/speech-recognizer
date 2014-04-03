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
 * Uses png++ library.
 * @see http://savannah.nongnu.org/projects/pngpp/
 */
void draw_png(wav_data_t* data, std::string file);

} // namespace wtm
} // namespace visual

#endif /* PNG_DRAWER_H_ */
