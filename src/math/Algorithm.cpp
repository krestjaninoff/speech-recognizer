#include "../config.h"
#include <assert.h>
#include <string>
#include <Algorithm.h>

namespace yazz {
namespace math {

const int32_t INVALID_INDEX = -1;
const double DEFAULT_PROBABILITY = 1e-4;

void Algorithm::initObservationsMap(map<observation_t, uint32_t>& observMap,
		const vector<observation_t>* sequence,
		const vector<observation_t>* observations, size_t observationsCnt) {

	for (vector<observation_t>::const_iterator iter = sequence->begin();
			iter != sequence->end(); iter++) {

		observation_t value = *iter;
		int32_t index = INVALID_INDEX;

		for (uint32_t i = 0; i < observationsCnt; i++) {
			if (0 == value.compare((*observations)[i])) {
				index = i;
				break;
			}
		}

		//assert("Unknown observation" && (INVALID_INDEX != index));
		observMap[value] = index;
	}

	if (DEBUG_ENABLED) {
		cout << "Observation map is: ";
		for (map<observation_t, uint32_t>::iterator iter = observMap.begin(); iter != observMap.end(); ++iter) {
			cout << (*iter).first << "->" << (*iter).second << " ";
		}
		cout << endl << endl;
	}
}

double Algorithm::getObservProb(observation_t observation, size_t stateId,
		double** emissions, map<observation_t, uint32_t>& observMap) {
	double probability = 0.0;

	assert("Observation not found" && observMap.count(observation));

	int32_t index = observMap[observation];
	if (index >= 0) {
		probability = emissions[stateId][index];

	// Even in case of unknown sybmol we still return very small probability,
	// that allows us to proceed the process when some sounds can't be recognised!
	} else {
		probability = DEFAULT_PROBABILITY;
	}

	return probability;
}

} /* namespace math */
} /* namespace yazz */
