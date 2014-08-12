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
	static double forward(const HmModel& model,	const vector<observation_t>* observations);
};

} /* namespace math */
} /* namespace yazz */

#endif /* FORWARDBACKWARD_H_ */
