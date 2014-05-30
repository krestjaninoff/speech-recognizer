#include <iostream>
#include <istream>
#include <ostream>
#include "Model.h"

using namespace std;

namespace yazz {
namespace math {

	Model::Model(string& text):
			text(text) {
		this->id = -1;
		this->samples = new vector<MFCCSample>();
	}

	Model::~Model() {
		delete this->samples;
	}

	void Model::addSample(double* mfcc, uint32_t size) {
		MFCCSample sample = {size, mfcc};
		this->samples->push_back(sample);
	}

	ostream& operator<<(ostream& fs, const Model& obj) {
		fs << obj.id;
		fs << obj.text;

		fs << obj.samples->size();

		for (vector<MFCCSample>::const_iterator sample = obj.samples->begin();
				sample != obj.samples->end(); ++sample) {

			fs.write((char*) sample->size, sizeof(uint32_t));
			fs.write((char*) sample->data, sizeof(double) * sample->size);
		}

		return fs;
	}

	istream& operator>>(istream& fs, Model& obj) {
		fs >> obj.id;
		fs >> obj.text;

		size_t size;
		fs >> size;

		MFCCSample sample;
		for (size_t i = 0; i < size; i++) {

			fs.read((char*)(&sample.size), sizeof(uint32_t));
			fs.read((char*)(sample.data), sizeof(double) * sample.size);

			obj.addSample(sample.data, sample.size);
		}

		return fs;
	}

} /* namespace math */
} /* namespace yazz */
