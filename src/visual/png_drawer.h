/*
 * A simple instrument for data visualization
 */
#ifndef PNG_DRAWER_H_
#define PNG_DRAWER_H_

#include <string>
#include "../audio/wav_reader.h"

namespace wtm {
namespace vizual {

/**
 * Draw PNG visualization for the data
 */
void draw_png(wav_data_t* data, string file);

}
}

#endif /* PNG_DRAWER_H_ */
