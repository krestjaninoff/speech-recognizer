#ifndef BAUMWELCH_H_
#define BAUMWELCH_H_

#include <Algorithm.h>

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
class BaumWelch : public Algorithm {
public:

	static void perform(HmModel* model, const vector<observation_t>* data);

private:

	static void algorithm(size_t stateCnt, size_t observationsCnt,
			double* initialDst, 	double** transitions, double** emissions,
			const vector<observation_t>* sequence,
			const vector<observation_t>* observations, map<observation_t, uint32_t> observMap,
			double** a, double** b, double** y, double*** e);

	static void calculateGamma(size_t stateCnt, const vector<observation_t>* sequence,
			double** a, double** b, double** y);

	static void calculateXi(size_t stateCnt, double** transitions, double** emissions,
			const vector<observation_t>* sequence,
			map<observation_t, uint32_t>& observMap, double** a, double** b,
			double*** e);

	static double updateModel(size_t stateCnt, size_t observationsCnt,
			double* initialDst, double** transitions, double** emissions,
			const vector<observation_t>* sequence,
			const vector<observation_t>* observations,
			double** y, double*** e);

	static uint32_t ITER_LIMIT;
	static double CONVERGENCE_EPSILON;
};

} /* namespace math */
} /* namespace yazz */

#endif /* BAUMWELCH_H_ */
