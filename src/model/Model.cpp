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
			MFCCSample theSample = *sample;

			fs.write((char*)(&theSample.size), sizeof(uint32_t));
			fs.write(reinterpret_cast<const char*>(theSample.data),
					std::streamsize(theSample.size * sizeof(double)));
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
			fs.read(reinterpret_cast<char*>(sample.data),
					std::streamsize(sample.size * sizeof(double)));

			obj.addSample(sample.data, sample.size);
		}

		return fs;
	}

} /* namespace math */
} /* namespace yazz */
