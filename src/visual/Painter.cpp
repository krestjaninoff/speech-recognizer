#include <cstdlib>
#include "png.h"
#include "assert.h"
#include <iostream>
#include "Painter.h"
#include "Frame.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace visual {

/**
 * Draw visualization of raw data
 */
void Painter::drawRawData(const SplitterPtr splitter, const string& file) {

	uint32_t imgWidth = IMAGE_WIDTH;
	uint32_t imgHeight = IMAGE_HEIGHT;

	uint32_t xMax = splitter->getWavData()->getNumberOfSamples();
	uint32_t yCorrection = splitter->getWavData()->getMinVal() < 0 ?
			-splitter->getWavData()->getMinVal() : 0;
	uint32_t yMax = splitter->getWavData()->getMaxVal() + yCorrection;

	uint32_t bufferSize = imgWidth * imgHeight * sizeof(uint8_t);
	uint8_t* image = (uint8_t*) malloc(bufferSize);
	if (image == NULL) {
		fprintf(stderr, "Could not create image buffer\n");
		return;
	}
	memset(image, 0, bufferSize);

	uint32_t xCurr = 0;
	for (vector<raw_t>::const_iterator yCurr = splitter->getWavData()->getRawData()->begin();
			yCurr != splitter->getWavData()->getRawData()->end(); ++yCurr) {

		// Contractive mapping
		uint32_t x = xCurr * (imgWidth - 1) / xMax;
		assert(x <= imgWidth);

		uint32_t y = (*yCurr + yCorrection) * (imgHeight - 1) / yMax;
		assert(y <= imgHeight);
		y = imgHeight - y - 1;

		uint32_t index = x + y * imgWidth;
		assert(index <= imgWidth * imgHeight);

		image[index] = true;
		xCurr++;
	}

	writeImage(file.c_str(), image, imgWidth, imgHeight);
	free(image);
}

/**
 * Draw visualization of frames
 */
void Painter::drawFrames(const SplitterPtr splitter, const string& file) {

	uint32_t imgWidth = IMAGE_WIDTH;
	uint32_t imgHeight = IMAGE_HEIGHT;

	uint32_t xMax = splitter->getFrames()->size();
	uint32_t yMax = splitter->getMaRMSMax();

	uint32_t bufferSize = imgWidth * imgHeight * sizeof(uint8_t);
	uint8_t* image = (uint8_t*) malloc(bufferSize);
	if (image == NULL) {
		fprintf(stderr, "Could not create image buffer\n");
		return;
	}
	memset(image, 0, bufferSize);

	// Draw maRMS for frames
	uint32_t xCurr = 0;
	for (vector<Frame*>::const_iterator frame = splitter->getFrames()->begin();
			frame != splitter->getFrames()->end(); ++frame) {

		// Contractive mapping
		uint32_t x = xCurr * (imgWidth - 1) / xMax;
		assert(x <= imgWidth);

		uint32_t y = ((*frame)->getMaRms()) * (imgHeight - 1) / yMax;
		assert(y <= imgHeight);
		y = imgHeight - y - 1;

		uint32_t index = x + y * imgWidth;
		assert(index <= imgWidth * imgHeight);

		if (splitter->isPartOfAWord(*frame)) {
			image[index] = 2;
		} else {
			image[index] = 1;
		}

		xCurr++;
	}

	// Draw word threshold
	uint32_t thresholdY = (splitter->getWordsThreshold()) * (imgHeight - 1) / yMax;
	thresholdY = imgHeight - thresholdY - 1;
	for (length_t x = 0; x < imgWidth; x++) {
		image[x + thresholdY * imgWidth] = 3;
	}

	writeImage(file.c_str(), image, imgWidth, imgHeight);
	free(image);
}

int Painter::writeImage(const char* filename, uint8_t* image, uint32_t width, uint32_t height) {
	int code = 0;
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep row;
	const short bytesPerPixel = 3;

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
	uint32_t rowSize;
	rowSize = bytesPerPixel * width * sizeof(png_byte);
	row = (png_bytep) malloc(rowSize);

	// Write image data
	uint32_t x, y;
	for (y = 0; y < height; y++) {
		memset(row, 0, rowSize);

		// TODO Refactor me
		for (x = 0; x < width; x ++) {
			if (1 == image[x + y * width]) {
				row[3*x] = 255;
				row[3*x + 1] = 0;
				row[3*x + 2] = 0;
			} else if (2 == image[x + y * width]) {
				row[3*x] = 0;
				row[3*x + 1] = 255;
				row[3*x + 2] = 0;
			} else if (3 == image[x + y * width]) {
				row[3*x] = 0;
				row[3*x + 1] = 0;
				row[3*x + 2] = 255;
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

