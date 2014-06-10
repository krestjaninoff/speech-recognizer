#ifndef DTW_H_
#define DTW_H_

#include <stdint.h>

using namespace std;

namespace yazz {
namespace math {

/**
 * Digital Time Warping
 *
 * @see http://sysmagazine.com/posts/135087/
 */
class DTW {
public:

	/**
	 * Calculate distance between two sequences of numbers
	 */
	static double calcDistance(double* seq1, uint32_t seq1size, double* seq2, uint32_t seq2size);

	/**
	 * Calculate distance between two sequences of vectors
	 */
	static double calcDistanceVector(double* seq1, uint32_t seq1size,
				double* seq2, uint32_t seq2size, uint8_t vectorSize);

private:

	static double findDistance(uint32_t seq1size, uint32_t seq2size, double** diffM);
};

} /* namespace math */
} /* namespace yazz */

#endif /* DTW_H_ */
