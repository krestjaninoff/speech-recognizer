#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits>
#include <Storage.h>
#include <Printer.h>
#include <HmModel.h>

using namespace yazz::math;

namespace yazz {
namespace model {

const string MODEL = "MODEL";
const string TEXT =	"TEXT";
const string STATES = "STATES";
const string OBSERVATIONS = "OBSERVATIONS";
const string INITIAL = "INITIAL";
const string TRANSITION = "TRANSITION";
const string EMISSION = "EMISSION";

const double MODEL_EPSILON = 1e-4; //numeric_limits<double>::epsilon()


HmModel::HmModel() {
	this->id = 0;

	this->states = NULL;
	this->stateCnt = 0;

	this->observations = NULL;
	this->observationCnt = 0;

	this->transitions = NULL;
	this->emissions = NULL;
	this->initialDst = NULL;

	this->text = "Undefined";
}

HmModel::~HmModel() {
	if (NULL != this->states) {
		delete this->states;
	}
	if (NULL != this->observations) {
		delete this->observations;
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

void HmModel::init(vector<state_t> states, const vector<observation_t> observations,
			double** transitions, double** emissions, double* initialDst, string text) {
	this->id = 0;

	this->states = new vector<state_t>(states);
	this->stateCnt = states.size();

	this->observations = new vector<observation_t>(observations);
	this->observationCnt = observations.size();

	this->transitions = transitions;
	this->emissions = emissions;
	this->initialDst = initialDst;

	this->text = text;

	this->check();
	this->normalize();
}

ostream& operator<<(ostream& fs, const HmModel& obj) {
	streamsize precisionOriginal = fs.precision(Storage::PRECISION);

	fs << MODEL << Storage::SPACE << obj.id << endl;
	fs << TEXT << Storage::SPACE << obj.text << endl;

	fs << STATES << Storage::TAB << Storage::TAB << obj.stateCnt;
	for (size_t i = 0; i < obj.stateCnt; i++) {
		fs << Storage::SPACE << obj.states->at(i);
	}
	fs << endl;

	fs << OBSERVATIONS << Storage::TAB << obj.observationCnt;
	for (size_t i = 0; i < obj.observationCnt; i++) {
		fs << Storage::SPACE << obj.observations->at(i);
	}
	fs << endl;

	fs << INITIAL << endl;
	for (size_t i = 0; i < obj.stateCnt; i++) {
		fs << obj.initialDst[i] << Storage::TAB;
	}
	fs << endl;

	fs << TRANSITION << endl;
	for (size_t i = 0; i < obj.stateCnt; i++) {
		for (size_t j = 0; j < obj.stateCnt; j++) {
			fs << obj.transitions[i][j] << Storage::TAB;
		}
		fs << endl;
	}
	fs << endl;

	fs << EMISSION << endl;
	for (size_t i = 0; i < obj.stateCnt; i++) {
		for (size_t j = 0; j < obj.observationCnt; j++) {
			fs << obj.emissions[i][j] << Storage::TAB;
		}
		fs << endl;
	}
	fs << endl;

	fs.precision(precisionOriginal);
	return fs;
}

istream& operator>>(istream& fs, HmModel& obj) {
	string tmpStr;

	// Model
	obj.id = Storage::readNamedInt(fs, MODEL, true);

	// Text
	obj.text = Storage::readNamedString(fs, TEXT);

	// States
	obj.stateCnt = Storage::readNamedInt(fs, STATES, true);

	vector<string>* statesTmp = new vector<string>();
	for (size_t i = 0; i < obj.stateCnt; i++) {
		fs >> tmpStr;
		statesTmp->push_back(tmpStr);
	}
	obj.states = statesTmp;

	// Observations
	obj.observationCnt = Storage::readNamedInt(fs, OBSERVATIONS, true);

	vector<string>* observationsTmp = new vector<string>();
	for (size_t i = 0; i < obj.observationCnt; i++) {
		fs >> tmpStr;
		observationsTmp->push_back(tmpStr);
	}
	obj.observations = observationsTmp;

	// Initial
	Storage::readHeader(fs, INITIAL);
	obj.initialDst = new double[obj.stateCnt];

	for (size_t i = 0; i < obj.stateCnt; i++) {
		if (!(fs >> obj.initialDst[i])) {
			cerr << "Invalid model: INITIAL data corrupted" << endl;
			exit(Storage::INVALID_CODE);
		}
	}

	// Transitions
	Storage::readHeader(fs, TRANSITION);
	obj.transitions = new double*[obj.stateCnt];

	for (size_t i = 0; i < obj.stateCnt; i++) {
		obj.transitions[i] = new double[obj.stateCnt];

		for (size_t j = 0; j < obj.stateCnt; j++) {
			if (!(fs >> obj.transitions[i][j])) {
				cerr << "Invalid model: TRANSITION data corrupted" << endl;
				exit(Storage::INVALID_CODE);
			}
		}
	}

	// Emissions
	Storage::readHeader(fs, EMISSION);
	obj.emissions = new double*[obj.stateCnt];

	for (size_t i = 0; i < obj.stateCnt; i++) {
		obj.emissions[i] = new double[obj.observationCnt];

		for (size_t j = 0; j < obj.observationCnt; j++) {
			if (!(fs >> obj.emissions[i][j])) {
				cerr << "Invalid model: EMISSION data corrupted" << endl;
				exit(Storage::INVALID_CODE);
			}
		}
	}

	return fs;
}

void HmModel::print() {

	cout << "[ID " << this->id << "] Model '" << this->text << "':";
	cout << endl << endl;

	cout << "States: ";
	Printer::printVector(*this->states, this->stateCnt);
	cout << endl;

	cout << "Observations: ";
	Printer::printVector(*this->observations, this->observationCnt);
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

		for (size_t j = 0; j < this->stateCnt; j++) {
			sum += this->transitions[i][j];
		}

		assert("Invalid transitions matrix: " && fabs(1 - sum) < MODEL_EPSILON);
	}

	// Check emission matrix
	for (size_t i = 0; i < this->stateCnt; i++) {
		sum = 0;

		for (size_t j = 0; j < this->observationCnt; j++) {
			sum += this->emissions[i][j];
		}

		assert("Invalid emissions matrix: " && fabs(1 - sum) < MODEL_EPSILON);
	}

	// Check initial state
	sum = 0;
	for (size_t i = 0; i < this->stateCnt; i++) {
		sum += this->initialDst[i];
	}
	assert("Invalid initial distribution" && fabs(1 - sum) < MODEL_EPSILON);
}

void HmModel::normalize() {

	normalizeVector(this->initialDst, this->stateCnt);

	for (size_t i = 0; i < this->stateCnt; i++) {
		normalizeVector(this->transitions[i], this->stateCnt);
	}
	for (size_t i = 0; i < this->stateCnt; i++) {
		normalizeVector(this->emissions[i], this->observationCnt);
	}
}

void HmModel::normalizeVector(double* vector, size_t size) {

	size_t zeroCnt = 0;
	for (size_t i = 0; i < size; i++) {

		if (vector[i] < MODEL_EPSILON) {
			zeroCnt++;
		}
	}

	double decreaseVal = zeroCnt * MODEL_EPSILON / (size - zeroCnt);
	for (size_t i = 0; i < size; i++) {

		if (vector[i] < MODEL_EPSILON) {
			vector[i] = MODEL_EPSILON;
		} else {
			vector[i] -= decreaseVal;
		}
	}
}

} /* namespace model */
} /* namespace yazz */
