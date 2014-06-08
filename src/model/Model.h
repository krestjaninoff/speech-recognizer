#ifndef MODEL_H_
#define MODEL_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace yazz {
namespace math {

/**
 * MFCC sample
 */
struct MFCCSample {
	uint32_t   size;
    double*	   data;
};

/**
 * Model is a set of MFCC and related text
 */
class Model {
public:

	Model(string& text);
	~Model();

	uint32_t getId() const { return this->id; }
	void setId(uint32_t id) { this->id = id; }

	const string& getText() const { return this->text; }

	void addSample(double* mfcc, uint32_t size);
	const vector<MFCCSample>& getSamples() const { return *this->samples; }


	friend std::ostream& operator<<(std::ostream& fs, const Model& obj);
	friend std::istream& operator>>(std::istream& fs, Model& obj);

private:
	uint32_t id;

	string& text;
	vector<MFCCSample>* samples;
};

} /* namespace math */
} /* namespace yazz */

#endif /* MODEL_H_ */
