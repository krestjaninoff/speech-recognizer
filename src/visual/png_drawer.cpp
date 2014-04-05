#include "png_drawer.h"
#include "png.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace visual {

/**
 * Draw PNG visualization for the data
 *
 * @see http://www.libpng.org/pub/png/book/
 */
void draw_data(wav_data_t* wav_data, string file) {

	size_t width = 1024;
	size_t height = 768;
	size_t max_x = wav_data->duration;
	size_t max_y = max(abs(wav_data->max_x), abs(wav_data->min_x));

	bool* buffer = (bool*) malloc(width * height * sizeof(bool));
	if (buffer == NULL) {
		fprintf(stderr, "Could not create image buffer\n");
		return;
	}

	int16_t it_x = 0;
	for (list<int16_t>::iterator it=wav_data->data->begin(); it != wav_data->data->end(); it_x++) {

		size_t x = it_x * width / max_x;
		size_t y = *it * height / max_y;

		buffer[y * width + x] = true;
	}

	write_image(file.c_str(), width, height, buffer);
	free(buffer);
}

int inline write_image(const char* filename, size_t width, size_t height, bool* buffer) {
	int code = 0;
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep row;

	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep) malloc(3 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	for (y=0 ; y<height ; y++) {
		for (x=0 ; x<width ; x++) {
			if (buffer[y * width + x]) {
				row[0] = 255;
				row[2] = 255;
				row[3] = 255;
			} else {
				row[0] = 0;
				row[2] = 0;
				row[3] = 0;
			}
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	// Clean up
	finalise:
		if (fp != NULL)
			fclose(fp);
		if (info_ptr != NULL)
			png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		if (png_ptr != NULL)
			png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		if (row != NULL)
			free(row);

	return code;
}

} // namespace visual
} // namespace wtm

