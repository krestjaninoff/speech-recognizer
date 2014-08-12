#ifndef BAUMWELCH_H_
#define BAUMWELCH_H_

#include <vector>
#include <map>
#include "../model/MfccEntry.h"
#include "../model/HmModel.h"

using namespace std;
using namespace yazz::model;

namespace yazz {
namespace math {

/**
 * BaumWelch algorithm implementation
 *
 * @see docs/baum_welch.pdf
 * @see https://www.youtube.com/playlist?list=PLD0F06AA0D2E8FFBA
 */
class BaumWelch {
public:

	static void perform(HmModel* model, const vector<observation_t>* data);

private:

	static double getObservProb(observation_t observation, uint32_t stateId,
			double** emissions, map<observation_t, uint32_t>& observMap);

	static void algorithm(size_t stateCnt, size_t observationsCnt, double* initialDst,
			double** transitions, double** emissions,
			const vector<observation_t>* data, observation_t* observations,
			map<observation_t, uint32_t> observMap, double** a, double** b,
			double** y, double*** e);

	static uint32_t ITER_LIMIT;
	static double CONVERGENCE_EPSILON;
};

} /* namespace math */
} /* namespace yazz */

#endif /* BAUMWELCH_H_ */
