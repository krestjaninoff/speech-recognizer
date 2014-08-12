#include <Algorithm.h>

namespace yazz {
namespace math {

void Algorithm::initObservationsMap(map<observation_t, uint32_t>& observMap,
		const vector<observation_t>* data,
		observation_t* observations, size_t observationsCnt) {

	for (vector<observation_t>::const_iterator iter = data->begin();
			iter != data->end(); iter++) {

		observation_t value = *iter;
		int32_t index = -1;

		for (uint32_t i = 0; i < observationsCnt; i++) {
			if (value == observations[i]) {
				index = i;
				break;
			}
		}

		observMap[value] = index;
	}
}

double Algorithm::getObservProb(observation_t observation, size_t stateId,
		double** emissions, map<observation_t, uint32_t>& observMap) {
	double probability = 0;

	int32_t index = observMap[observation];
	if (index > 0) {
		probability = emissions[stateId][index];
	}

	return probability;
}

} /* namespace math */
} /* namespace yazz */
