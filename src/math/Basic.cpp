#include <Basic.h>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <stdlib.h>

using namespace std;

namespace yazz {
namespace math {

double Basic::rms(const raw_t* source, uint32_t start, uint32_t finish) {
	double value = 0;

	for (uint32_t i = start; i <= finish; i++) {
		value += source[i] * source[i];
	}
	value /= (finish - start + 1);

	return sqrt(value);
}

double Basic::entropy(const double* source, uint32_t start, uint32_t finish,
		uint8_t binsCount, double minRaw, double maxRaw) {
	double entropy = 0;

	double binSize = abs(maxRaw - minRaw) / static_cast<double>(binsCount);
	if (fabs(binSize) < numeric_limits<double>::epsilon()) {
		return 0;
	}

	double* p = new double[binsCount];
	for (uint8_t i = 0; i < binsCount; i++) {
		p[i] = 0.;
	}

	// Calculate probabilities
	uint8_t index;
	for (uint32_t i = start; i <= finish; i++) {
		double value = source[i];
		index = floor((value - minRaw) / binSize);

		if (index >= binsCount) {
			index = binsCount - 1;
		}

		p[index] += 1.;
	}

	// Normalize probabilities
	uint8_t size = finish - start + 1;
	for (uint8_t i = 0; i < binsCount; i++) {
		p[i] /= size;
	}

	// Calculate entropy
	for (uint8_t i = 0; i < binsCount; i++) {
		if (p[i] > numeric_limits<double>::epsilon()) {
			entropy += p[i] * log2(p[i]);
		}
	}

	delete [] p;

	entropy = -entropy;
	return entropy;
}

double Basic::euclideanDistance(const double* a, const double* b, size_t size) {
	double distance = 0;

	for (size_t i = 0; i < size; i++) {
		distance += pow((a[i] - b[i]), 2);
	}

	return sqrt(distance);
}

double Basic::euclideanDistanceWithWeights(const double* a, const double* b, const double* weights, size_t size) {
	double distance = 0;

	for (size_t i = 0; i < size; i++) {
		distance += pow((a[i] - b[i]), 2) * weights[i];
	}

	return sqrt(distance);
}

} /* namespace math */
} /* namespace yazz */
