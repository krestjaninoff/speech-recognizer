#include "png_drawer.h"
#include <pngwriter.h>

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace vizual {

/**
 * Draw PNG visualization for the data
 */
void draw_data(wav_data_t* data, string file) {
	int size_x = 500;
	int size_y = 500;

	pngwriter png(size_x, size_y, 0, file);

	for (wav_data_t::iterator it=data->begin(); it != data->end(); ++it) {
		png.plot(i, y, 0.0, 0.0, 1.0);
	}

	png.close();
}

} // namespace
} // namespace

