#ifndef DTW_H_
#define DTW_H_

#include <stdint.h>
#include <vector>
#include <math.h>
#include "maph.h"

using namespace std;

namespace wtm {
namespace math {

/**
 * Digital Time Warping
 *
 * @see http://www.fon.hum.uva.nl/david/ba_shs/2010/SpraakherkenningEnSynthese_week08.pdf
 */
class DTW {
public:

	static double a() {return 5.;}

	/**
	 * Calculate distance between two sequences
	 */
	static double calcDistance(double* seq1, uint32_t seq1size, double* seq2, uint32_t seq2size);
};

} /* namespace math */
} /* namespace wtm */

#endif /* DTW_H_ */
