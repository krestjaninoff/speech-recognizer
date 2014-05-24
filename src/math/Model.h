#ifndef MODEL_H_
#define MODEL_H_

#include <string>
#include <vector>
#include "maph.h"

using namespace std;

namespace wtm {
namespace math {

/**
 * MFCC sample
 */
struct MFCCSample {
	uint32_t            size;
    double*	            data;
};

/**
 * Model is a set of MFCC and related text
 */
class Model {
public:

	Model(string& text);
	~Model();

	const uint32_t getId() { return this->id; }
	void setId(uint32_t id) { this->id = id; }

	const string& getText() { return this->text; }

	void addSample(double* mfcc, uint32_t size);
	const vector<MFCCSample>& getSamples() { return *this->samples; }

	friend ofstream& operator<< (std::ofstream &f, const Model &obj);
	friend ifstream& operator>> (std::ofstream &f, Model &obj);

private:
	uint32_t id;

	string& text;
	vector<MFCCSample>* samples;
};

} /* namespace math */
} /* namespace wtm */

#endif /* MODEL_H_ */
