#ifndef FORWARDBACKWARD_H_
#define FORWARDBACKWARD_H_

#include <vector>
#include "../model/HmModel.h"

using namespace std;
using namespace yazz::model;

namespace yazz {
namespace math {

/**
 * ForwardBackward algorithm implementation
 */
class ForwardBackward {
public:

	static const HmModel* perform(const vector<HmModel*>* models,
			const vector<observation_t>* observations);
};

} /* namespace math */
} /* namespace yazz */

#endif /* FORWARDBACKWARD_H_ */
