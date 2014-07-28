#ifndef HMMMODEL_H_
#define HMMMODEL_H_

#include <iostream>
#include <string>

using namespace std;

namespace yazz {
namespace model {

// Define special types for HMM states and observations
typedef char state_t;
typedef char observation_t;

/**
 * Hidden Markov Model
 *
 * @see https://www.youtube.com/watch?v=7KGdE2AK_MQ&index=95&list=PLD0F06AA0D2E8FFBA
 */
class HmModel {
public:
	HmModel();
	~HmModel();

	void init(state_t* states, size_t stateCnt, observation_t* observations, size_t observationCnt,
			double** transitions, double** emissions, double* initialDst, string& text);

	uint32_t getId() const { return this->id; }
	void setId(uint32_t id) { this->id = id; }

	state_t* getStates() const { return this->states; }
	size_t getStateCnt() const { return this->stateCnt; }

	observation_t* getObservations() const { return this->observations; }
	size_t getObservationCnt() const { return this->observationCnt; }

	double** getTransitions() const { return this->transitions; }
	double** getEmissions() const { return this->emissions; }
	double* getInitialDst() const { return this->initialDst; }

	const string getText() const { return this->text; }

	friend std::ostream& operator<<(std::ostream& fs, const HmModel& obj);
	friend std::istream& operator>>(std::istream& fs, HmModel& obj);

private:
	uint32_t id;

	state_t* states;
	size_t stateCnt;

	observation_t* observations;
	size_t observationCnt;

	double** transitions;	// stateCnt * stateCnt
	double** emissions;		// stateCnt * observationCnt
	double* initialDst;		// stateCnt

	string text;
};

} /* namespace model */
} /* namespace yazz */

#endif /* HMMMODEL_H_ */
