#include <iostream>
#include <istream>
#include <ostream>
#include "Model.h"

namespace wtm {
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

	ostream& operator<< (std::ostream &fs, const Model &obj) {
		obj.id >> fs;
		obj.text >> fs;

		obj.samples->size() >> fs;

		for (vector<MFCCSample>::const_iterator sample = obj.samples->begin();
				sample != obj.samples->end(); ++sample) {

			fs.write((char*) sample->size, sizeof(uint32_t));
			fs.write((char*) sample->data, sizeof(double) * sample->size);
		}

		return fs;
	}

	istream& operator>> (std::istream &fs, Model &obj) {
		obj.id << fs;
		obj.text << fs;

		size_t size;
		size << fs;

		MFCCSample sample;
		for (size_t i = 0; i < size; i++) {

			fs.read((char*)(&sample.size), sizeof(uint32_t));
			fs.read((char*)(sample.data), sizeof(double) * sample.size);

			obj.addSample(sample.data, sample.size);
		}

		return fs;
	}

} /* namespace math */
} /* namespace wtm */
