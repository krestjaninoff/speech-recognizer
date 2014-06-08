#include <Model.h>
#include <stddef.h>
#include <cstdint>
#include <iostream>
#include <iterator>

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

		fs.write((char*) &obj.id, sizeof(uint32_t));

		size_t textSize = obj.text.size();
		fs.write((char*) &textSize, sizeof(size_t));
		fs.write(&obj.text[0], sizeof(char) * textSize);

		size_t samplesCnt = obj.samples->size();
		fs.write((char*) &samplesCnt, sizeof(size_t));

		for (vector<MFCCSample>::const_iterator sample = obj.samples->begin();
				sample != obj.samples->end(); ++sample) {
			MFCCSample theSample = *sample;

			fs.write((char*)(&theSample.size), sizeof(uint32_t));
			fs.write(reinterpret_cast<char*>(theSample.data),
					streamsize(theSample.size * sizeof(double)));
		}

		return fs;
	}

	istream& operator>>(istream& fs, Model& obj) {

		fs.read((char*)(&obj.id), sizeof(uint32_t));

		size_t textSize;
		fs.read((char*) &textSize, sizeof(size_t));

		// Yes, it's really ugly. One day I'll refactor this mess.
		char* text = new char[textSize + 1];
		fs.read(text, sizeof(char) * textSize);
		text[textSize] = '\0';
		string textString(text);
		obj.text = textString;

		size_t samplesCnt;
		fs.read((char*) &samplesCnt, sizeof(uint32_t));

		MFCCSample sample;
		for (size_t i = 0; i < samplesCnt; i++) {

			fs.read((char*)(&sample.size), sizeof(uint32_t));

			sample.data = new double[sample.size];
			fs.read(reinterpret_cast<char*>(sample.data),
					streamsize(sample.size * sizeof(double)));

			obj.addSample(sample.data, sample.size);
		}

		return fs;
	}

} /* namespace math */
} /* namespace yazz */
