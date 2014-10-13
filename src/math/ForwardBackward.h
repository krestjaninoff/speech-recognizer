#ifndef FORWARDBACKWARD_H_
#define FORWARDBACKWARD_H_

#include <Algorithm.h>

using namespace std;
using namespace yazz::model;

namespace yazz {
namespace math {

/**
 * ForwardBackward algorithm implementation
 *
 * @see docs/hmm_pdmi.pdf
 */
class ForwardBackward : public Algorithm {
public:

	/**
	 * Calculate probability of model with given observations
	 */
	static double calcPossibility(const HmModel& model,	const vector<observation_t>* sequence);

	/**
	 * Forward algorithm
	 * <p>
	 * Calculates A matrix.
	 */
	static void forwardAlgorithm(size_t stateCnt,
			double* initialDst,	double** transitions, double** emissions,
			const vector<observation_t>* sequence, double** a,
			map<observation_t, uint32_t>& observMap);

	/**
	 * Backward algorithm
	 * <p>
	 * Calculates B matrix.
	 */
	static void backwardAlgorithm(size_t stateCnt,
			double** transitions, double** emissions,
			const vector<observation_t>* sequence, double** b,
			map<observation_t, uint32_t>& observMap);
};

} /* namespace math */
} /* namespace yazz */

#endif /* FORWARDBACKWARD_H_ */
