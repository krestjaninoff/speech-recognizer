/**
 * Common math settings
 */

#ifndef MATF_H_
#define MATF_H_

#include "../common.h"

namespace wtm {
namespace math {

	/**
	 * MFCC vector size
	 */
	const short MFCC_SIZE = 12;

	/**
	 * Frequency bounds
	 */
	const short FREQ_MIN = 300;
	const short FREQ_MAX = 8000;

	/**
	 * Math constants
	 */
	const double PI = 3.14159265358979323846;
	const double EXP = 2.7182818284590452354;

} // namespace math
} // namespace wtm

#endif /* MATF_H_ */
