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
void draw_data(wav_data_t* data, string file) {

	int size_x = 1024;
	int size_y = 768;

	image img(size_x, size_y);

	int it_x = 0;
	for (wav_data_t::iterator it=data->begin(); it != data->end(); it_x++) {

		// TODO We need to know max X,Y to implement contractive mapping
		int x = it_x / size_x;
		int y = *it / size_y;

		rgb_pixel pixel = basic_rgb_pixel(0, 0, 0);
		img.set_pixel(x, y, pixel);
	}

	img.write(file);
}

} // namespace
} // namespace

