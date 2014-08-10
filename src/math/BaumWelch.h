#ifndef BAUMWELCH_H_
#define BAUMWELCH_H_

#include <vector>
#include "../model/MfccEntry.h"
#include "../model/HmModel.h"

using namespace std;
using namespace yazz::model;

namespace yazz {
namespace math {

/**
 * BaumWelch algorithm implementation
 */
class BaumWelch {
public:

	static void perform(HmModel* model, const vector<observation_t>* data);
};

} /* namespace math */
} /* namespace yazz */

#endif /* BAUMWELCH_H_ */
