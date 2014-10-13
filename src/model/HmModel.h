#ifndef HMMODEL_H_
#define HMMODEL_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace yazz {
namespace model {

// Define special types for HMM states and observations
typedef string state_t;
typedef string observation_t;

/**
 * Hidden Markov Model
 *
 * @see https://www.youtube.com/watch?v=7KGdE2AK_MQ&index=95&list=PLD0F06AA0D2E8FFBA
 */
class HmModel {
public:
	HmModel();
	~HmModel();

	void init(vector<state_t> states, vector<observation_t>,
			double** transitions, double** emissions, double* initialDst,
			string text);

	uint32_t getId() const { return this->id; }
	void setId(uint32_t id) { this->id = id; }

	const vector<state_t>* getStates() const { return this->states; }
	size_t getStateCnt() const { return this->stateCnt; }

	const vector<observation_t>* getObservations() const { return this->observations; }
	size_t getObservationCnt() const { return this->observationCnt; }

	double** getTransitions() const { return this->transitions; }
	double** getEmissions() const { return this->emissions; }
	double* getInitialDst() const { return this->initialDst; }

	string getText() const { return this->text; }

	friend std::ostream& operator<<(std::ostream& fs, const HmModel& obj);
	friend std::istream& operator>>(std::istream& fs, HmModel& obj);

	void print();

private:
	uint32_t id;

	const vector<state_t>* states;
	size_t stateCnt;

	const vector<observation_t>* observations;
	size_t observationCnt;

	double** transitions;	// stateCnt * stateCnt
	double** emissions;		// stateCnt * observationCnt
	double* initialDst;		// stateCnt

	string text;

	void check();

	void normalize();
	static void normalizeVector(double* vector, size_t size);
};

} /* namespace model */
} /* namespace yazz */

#endif /* HMMODEL_H_ */
