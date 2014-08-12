#ifndef BASIC_MATH_H_
#define BASIC_MATH_H_

#include "../config.h"
#include <stdint.h>

using namespace std;

namespace yazz {
namespace math {

/**
 * Common math
 */
class Basic {
public:

	/**
	 * Calculate Root Mean Square
	 * @see http://en.wikipedia.org/wiki/Root_mean_square
	 */
	static double rms(const raw_t* source, uint32_t start, uint32_t finish);

	/**
	 * Calculate entropy for data sequence
	 * @see http://en.wikipedia.org/wiki/Entropy_(information_theory)
	 */
	static double entropy(const double* source, uint32_t start, uint32_t finish,
			uint8_t binsCount, double minRaw, double maxRaw);

	static double euclideanDistance(double* a, double* b, size_t size);

	// Print helpers
	static void printVector(char* vector, size_t size);
	static void printVector(double* vector, size_t size);
	static void printMatrix(double** matrix, size_t sizeX, size_t sizeY);
	static void printMatrix3D(double*** matrix, size_t sizeX, size_t sizeY, size_t sizeZ);
};

} /* namespace math */
} /* namespace yazz */

#endif /* BASIC_MATH_H_ */
