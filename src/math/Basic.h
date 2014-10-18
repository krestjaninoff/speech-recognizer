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

	static double euclideanDistance(const double* a, const double* b, size_t size);
	static double euclideanDistanceWithWeights(const double* a, const double* b, const double* weights, size_t size);
};

} /* namespace math */
} /* namespace yazz */

#endif /* BASIC_MATH_H_ */
