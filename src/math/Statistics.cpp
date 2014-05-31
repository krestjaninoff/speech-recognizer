#include <Statistics.h>

namespace yazz {
namespace math {

	double Statistics::rms(const std::vector<raw_t>& source, uint32_t start, uint32_t finish) {
		double value = 0;

		for (uint32_t i = start; i <= finish; i++) {
			value += source.at(i) * source.at(i);
		}
		value /= (finish - start + 1);

		return sqrt(value);
	}

	double Statistics::entropy(const std::vector<raw_t>& source, uint32_t start, uint32_t finish,
			uint8_t binsCount, raw_t minRaw, raw_t maxRaw) {
		double entropy = 0;

		double binSize = abs(maxRaw - minRaw) / (double) binsCount;
		if (binSize <= 1) {
			return 0;
		}

		double* p = new double[binsCount];
		for (uint8_t i = 0; i < binsCount; i++) {
			p[i] = 0.;
		}

		// Calculate probabilities
		int index;
		for (uint32_t i = start; i <= finish; i++) {
			raw_t value = source.at(i);
			index = floor((value - minRaw) / binSize);

			if (index >= binsCount) {
				index = binsCount - 1;
			}

			p[index] += 1.;
		}

		// Normalize probabilities
		uint8_t size = finish - start + 1;
		double maxProbability = p[0];
		if (maxProbability > 0) {
			for (uint8_t i = 0; i < binsCount; i++) {
				p[i] /= size;
			}
		}

		// Calculate entropy
		for (uint8_t i = 0; i < binsCount; i++) {
			if (p[i] > 0) {
				entropy += p[i] * log2(p[i]);
			}
		}

		delete [] p;

		entropy = -entropy;
		return entropy;
	}

} /* namespace math */
} /* namespace yazz */
