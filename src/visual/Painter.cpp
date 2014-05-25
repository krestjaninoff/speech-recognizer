#include <stdio.h>
#include <cstdlib>
#include "assert.h"
#include <iostream>
#include "png.h"
#include "Painter.h"
#include "Frame.h"

using namespace std;
using namespace yazz::audio;

namespace yazz {
namespace visual {

enum Color { BLACK, RED, GREEN, BLUE };

/**
 * Draw visualization of raw data
 */
void Painter::drawRawData(const Processor* processor, const string& file) {

	uint32_t imgWidth = IMAGE_WIDTH;
	uint32_t imgHeight = IMAGE_HEIGHT;

	uint32_t xMax = processor->getWavData()->getNumberOfSamples();
	uint32_t yCorrection = processor->getWavData()->getMinVal() < 0 ?
			-processor->getWavData()->getMinVal() : 0;
	uint32_t yMax = processor->getWavData()->getMaxVal() + yCorrection;

	uint32_t bufferSize = imgWidth * imgHeight * sizeof(uint8_t);
	uint8_t* image = (uint8_t*) malloc(bufferSize);
	if (image == NULL) {
		cerr << "Could not create image buffer" << endl;
		return;
	}
	memset(image, 0, bufferSize);

	uint32_t xCurr = 0;
	for (vector<raw_t>::const_iterator yCurr = processor->getWavData()->getRawData()->begin();
			yCurr != processor->getWavData()->getRawData()->end(); ++yCurr) {

		// Contractive mapping
		uint32_t x = xCurr * (imgWidth - 1) / xMax;
		assert(x <= imgWidth);

		uint32_t y = (*yCurr + yCorrection) * (imgHeight - 1) / yMax;
		assert(y <= imgHeight);
		y = imgHeight - y - 1;

		uint32_t index = x + y * imgWidth;
		assert(index <= imgWidth * imgHeight);

		image[index] = GREEN;
		xCurr++;
	}

	writeImage(file.c_str(), image, imgWidth, imgHeight);
	free(image);
}

/**
 * Draw visualization of frames
 */
void Painter::drawFrames(const Processor* processor, const string& file) {

	uint32_t imgWidth = IMAGE_WIDTH;
	uint32_t imgHeight = IMAGE_HEIGHT;

	uint32_t xMax = processor->getFrames()->size();
	uint32_t yMax = processor->getMaRMSMax();

	uint32_t bufferSize = imgWidth * imgHeight * sizeof(uint8_t);
	uint8_t* image = (uint8_t*) malloc(bufferSize);
	if (image == NULL) {
		cerr << "Could not create image buffer" << endl;
		return;
	}
	memset(image, 0, bufferSize);

	// Draw maRMS for frames
	uint32_t xCurr = 0;
	for (vector<Frame*>::const_iterator frame = processor->getFrames()->begin();
			frame != processor->getFrames()->end(); ++frame) {

		// Contractive mapping
		uint32_t x = xCurr * (imgWidth - 1) / xMax;
		assert(x <= imgWidth);

		uint32_t y = ((*frame)->getMaRms()) * (imgHeight - 1) / yMax;
		assert(y <= imgHeight);
		y = imgHeight - y - 1;

		uint32_t index = x + y * imgWidth;
		assert(index <= imgWidth * imgHeight);

		if (processor->isPartOfAWord(**frame)) {
			image[index] = GREEN;
		} else {
			image[index] = RED;
		}

		xCurr++;
	}

	// Draw word threshold
	uint32_t thresholdY = (processor->getWordsThreshold()) * (imgHeight - 1) / yMax;
	thresholdY = imgHeight - thresholdY - 1;
	for (length_t x = 0; x < imgWidth; x++) {
		image[x + thresholdY * imgWidth] = BLUE;
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
		cerr << "Could not open file %s for writing" << endl;
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		cerr << "Could not allocate write struct" << endl;
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		cerr << "Could not allocate info struct" << endl;
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		cerr << "Error during png creation" << endl;
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

		for (x = 0; x < width; x ++) {
			if (RED == image[x + y * width]) {
				row[3*x] = 255;
				row[3*x + 1] = 0;
				row[3*x + 2] = 0;
			} else if (GREEN == image[x + y * width]) {
				row[3*x] = 0;
				row[3*x + 1] = 255;
				row[3*x + 2] = 0;
			} else if (BLUE == image[x + y * width]) {
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
} // namespace yazz

