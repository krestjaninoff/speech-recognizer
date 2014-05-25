#ifndef RECOGNIZER_H_
#define RECOGNIZER_H_

#include <string>
#include <vector>
#include "maph.h"
#include "Model.h"
#include "../audio/Word.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace math {

/**
 * Recognizer tries to determine which model suits better for the specific word
 */
class Recognizer {
public:

	Recognizer(vector<Model*>* models);
	~Recognizer();

	const Model* recognize(const Word& word);

private:
	const vector<Model*>* models;
};

} /* namespace math */
} /* namespace wtm */

#endif /* RECOGNIZER_H_ */
