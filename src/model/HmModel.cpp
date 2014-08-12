#include <assert.h>
#include <math.h>
#include <limits>
#include <../math/Printer.h>
#include <HmModel.h>

using namespace yazz::math;

namespace yazz {
namespace model {

HmModel::HmModel() {
	this->id = 0;

	this->states = NULL;
	this->stateCnt = 0;

	this->observations = NULL;
	this->observationCnt = 0;

	this->transitions = NULL;
	this->emissions = NULL;
	this->initialDst = NULL;

	this->text = '\0';
}

HmModel::~HmModel() {
	if (NULL != this->states) {
		delete [] this->states;
	}
	if (NULL != this->observations) {
		delete [] this->observations;
	}

	if (NULL != this->transitions) {
		for (size_t i = 0; i < this->stateCnt; i++) {
			delete [] this->transitions[i];
		}

		delete [] this->transitions;
	}

	if (NULL != this->emissions) {
		for (size_t i = 0; i < this->stateCnt; i++) {
			delete [] this->emissions[i];
		}
	}

	if (NULL != this->initialDst) {
		delete [] this->initialDst;
	}
}

void HmModel::init(state_t* states, size_t stateCnt, observation_t* observations, size_t observationCnt,
			double** transitions, double** emissions, double* initialDst, string text) {
	this->id = 0;

	this->states = states;
	this->stateCnt = stateCnt;

	this->observations = observations;
	this->observationCnt = observationCnt;

	this->transitions = transitions;
	this->emissions = emissions;
	this->initialDst = initialDst;

	this->text = text;

	this->check();
}

ostream& operator<<(ostream& fs, const HmModel& obj) {

	fs.write((char*) &obj.id, sizeof(uint32_t));

	size_t textSize = obj.text.size();
	fs.write((char*) &textSize, sizeof(size_t));
	fs.write(&obj.text[0], sizeof(char) * textSize);

	fs.write((char*)(&obj.stateCnt), sizeof(size_t));
	fs.write(reinterpret_cast<char*>(obj.states),
			streamsize(obj.stateCnt * sizeof(state_t)));

	fs.write((char*)(&obj.observationCnt), sizeof(size_t));
	fs.write(reinterpret_cast<char*>(obj.observations),
			streamsize(obj.observationCnt * sizeof(observation_t)));

	for (size_t i = 0; i < obj.stateCnt; i++) {
		fs.write(reinterpret_cast<char*>(obj.transitions[i]),
				streamsize(obj.stateCnt * sizeof(double)));
	}

	for (size_t i = 0; i < obj.stateCnt; i++) {
		fs.write(reinterpret_cast<char*>(obj.emissions[i]),
				streamsize(obj.observationCnt * sizeof(double)));
	}

	fs.write(reinterpret_cast<char*>(obj.initialDst),
			streamsize(obj.stateCnt * sizeof(double)));

	return fs;
}

istream& operator>>(istream& fs, HmModel& obj) {

	fs.read((char*)(&obj.id), sizeof(uint32_t));

	size_t textSize;
	fs.read((char*) &textSize, sizeof(size_t));

	// Yes, it's really ugly. One day I'll refactor this mess.
	char* text = new char[textSize + 1];
	fs.read(text, sizeof(char) * textSize);
	text[textSize] = '\0';
	string textString(text);
	obj.text = textString;

	fs.read((char*)(&obj.stateCnt), sizeof(size_t));
	obj.states = new state_t[obj.stateCnt];
	fs.read(reinterpret_cast<char*>(obj.states),
			streamsize(obj.stateCnt * sizeof(state_t)));

	fs.read((char*)(&obj.observationCnt), sizeof(size_t));
	obj.observations = new observation_t[obj.observationCnt];
	fs.read(reinterpret_cast<char*>(obj.observations),
			streamsize(obj.observationCnt * sizeof(observation_t)));

	obj.transitions = new double*[obj.stateCnt];
	for (size_t i = 0; i < obj.stateCnt; i++) {
		obj.transitions[i] = new double[obj.stateCnt];

		fs.read(reinterpret_cast<char*>(obj.transitions[i]),
			streamsize(obj.stateCnt * sizeof(double)));
	}

	obj.emissions = new double*[obj.stateCnt];
	for (size_t i = 0; i < obj.stateCnt; i++) {
		obj.emissions[i] = new double[obj.observationCnt];

		fs.read(reinterpret_cast<char*>(obj.emissions[i]),
			streamsize(obj.observationCnt * sizeof(double)));
	}

	obj.initialDst = new double[obj.stateCnt];
	fs.read(reinterpret_cast<char*>(obj.initialDst),
			streamsize(obj.stateCnt * sizeof(double)));

	return fs;
}

void HmModel::print() {

	cout << "Model '" << this->text << "' (id: " << this->id << ")";
	cout << endl << endl;

	cout << "States: ";
	Printer::printVector(this->states, this->stateCnt);
	cout << endl << endl;

	cout << "Observations: ";
	Printer::printVector(this->observations, this->observationCnt);
	cout << endl << endl;

	cout << "Transitions:" << endl;
	Printer::printMatrix(this->transitions, this->stateCnt, this->stateCnt);
	cout << endl;

	cout << "Emissions:" << endl;
	Printer::printMatrix(this->emissions, this->stateCnt, this->observationCnt);
	cout << endl;

	cout << "Initial distribution:" << endl;
	Printer::printVector(this->initialDst, this->stateCnt);
	cout << endl;
}

void HmModel::check() {
	double sum;

	// Check transition matrix
	for (size_t i = 0; i < this->stateCnt; i++) {
		sum = 0;

		for (size_t j = 0; j < this->observationCnt; j++) {
			sum += this->transitions[i][j];
		}

		assert(fabs(1 - sum) > numeric_limits<double>::epsilon());
	}

	// Check emission matrix
	for (size_t i = 0; i < this->stateCnt; i++) {
		sum = 0;

		for (size_t j = 0; j < this->observationCnt; j++) {
			sum += this->emissions[i][j];
		}

		assert(fabs(1 - sum) > numeric_limits<double>::epsilon());
	}

	// Check initial state
	sum = 0;
	for (size_t i = 0; i < this->stateCnt; i++) {
		sum += this->initialDst[i];
	}
	assert(fabs(1 - sum) > numeric_limits<double>::epsilon());
}

} /* namespace model */
} /* namespace yazz */
