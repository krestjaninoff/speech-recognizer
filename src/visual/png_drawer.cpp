#include "png_drawer.h"
#include "png++/png.hpp"

using namespace std;
using namespace wtm::audio;

using namespace png;

namespace wtm {
namespace visual {

/**
 * Draw PNG visualization for the data
 *
 * @see http://fedetft.wordpress.com/2010/10/05/handling-png-images-in-cpp/
 */
void draw_data(wav_data_t* wav_data, string file) {

	size_t size_x = 1024;
	size_t size_y = 768;
	size_t max_x = wav_data->duration;
	size_t max_y = max(abs(wav_data->max_x), abs(wav_data->min_x));

	image img(size_x, size_y);

	int16_t it_x = 0;
	for (list<int16_t>::iterator it=wav_data->data->begin(); it != wav_data->data->end(); it_x++) {

		size_t x = it_x * size_x / max_x;
		size_t y = *it * size_y / max_y;

		rgb_pixel pixel = basic_rgb_pixel(0, 0, 0);

		img.set_pixel(x, y, pixel);
	}

	img.write(file.c_str());
}

} // namespace
} // namespace

