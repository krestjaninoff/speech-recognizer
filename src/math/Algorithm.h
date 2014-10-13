#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <vector>
#include <map>
#include "../model/HmModel.h"

using namespace std;
using namespace yazz::model;

namespace yazz {
namespace math {

/**
 * Abstract algorithm
 */
class Algorithm {

protected:

	static void initObservationsMap(map<observation_t, uint32_t>& observMap,
			const vector<observation_t>* sequence,
			const vector<observation_t>* observations, size_t observationsCnt);

	static double getObservProb(observation_t observation, uint32_t stateId,
			double** emissions, map<observation_t, uint32_t>& observMap);
};

} /* namespace math */
} /* namespace yazz */

#endif /* ALGORITHM_H_ */
