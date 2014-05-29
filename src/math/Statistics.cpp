#include <Statistics.h>

namespace yazz {
namespace math {

	double Statistics::rms(const std::vector<raw_t>& source, uint32_t start, uint32_t finish) {
		double value = 0;

		for (uint32_t i = start; i < finish; i++) {
			value += source.at(i) * source.at(i);
		}
		value /= (finish - start);

		return sqrt(value);
	}

	double Statistics::entropy(const std::vector<raw_t>& source, uint32_t start, uint32_t finish,
			ushort binsCount, raw_t minRaw, raw_t maxRaw) {
		double entropy = 0;

		int binSize = abs(minRaw - maxRaw) / binsCount + 1;
		if (1 == binSize) {
			return 0;
		}

		double p[binsCount];
		for (ushort i = 0; i < binsCount; i++) {
			p[i] = 0;
		}

		// Calculate probabilities
		int index;
		for (uint32_t i = start; i < finish; i++) {
			raw_t value = source.at(i);
			index = round((value - minRaw) / binSize);
			p[index] += 1.;
		}

		// Normalize probabilities
		double maxProbability = p[0];
		for (ushort i = 1; i < binsCount; i++) {
			maxProbability = std::max(maxProbability, p[i]);
		}
		if (maxProbability > 0) {
			for (ushort i = 0; i < binsCount; i++) {
				p[i] /= maxProbability;
			}
		}

		// Calculate entropy
		for (ushort i = 0; i < binsCount; i++) {
			if (p[i] > 0) {
				entropy += p[i] * log2(p[i]);
			}
		}

		entropy = -entropy;
		return entropy;
	}

} /* namespace math */
} /* namespace yazz */
