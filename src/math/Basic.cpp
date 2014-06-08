#include <Basic.h>
#include <cmath>
#include <cstdlib>
#include <limits>

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

		double p[binsCount];
		for (uint8_t i = 0; i < binsCount; i++) {
			p[i] = 0.;
		}

		// Calculate probabilities
		int index;
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

		entropy = -entropy;
		return entropy;
	}

} /* namespace math */
} /* namespace yazz */
