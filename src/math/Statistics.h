#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <stdint.h>
#include <vector>
#include <math.h>
#include "maph.h"
#include "../audio/audio.h"

using namespace std;
using namespace yazz::audio;

namespace yazz {
namespace math {

/**
 * Statistics related calculations
 */
class Statistics {
public:

	/**
	 * Calculate Root Mean Square
	 * @see http://en.wikipedia.org/wiki/Root_mean_square
	 */
	static double rms(const std::vector<raw_t>& source, uint32_t start, uint32_t finish);

	/**
	 * Calculate entropy for data sequence
	 * @see http://en.wikipedia.org/wiki/Entropy_(information_theory)
	 */
	static double entropy(const std::vector<raw_t>& source, uint32_t start, uint32_t finish,
			uint8_t binsCount, raw_t minRaw, raw_t maxRaw);
};

} /* namespace math */
} /* namespace yazz */

#endif /* STATISTICS_H_ */
