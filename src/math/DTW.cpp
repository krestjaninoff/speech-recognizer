#include <assert.h>
#include <DTW.h>

namespace wtm {
namespace math {

	double DTW::calcDistance(double* seq1, uint32_t seq1size, double* seq2, uint32_t seq2size) {

		// Create diff matrix

		double** diffM = new double*[seq1size];
		for (uint32_t i = 0; i < seq1size; i++) {
			diffM[i] = new  double[seq2size];
		}

		for (uint32_t i = 0; i < seq1size; i++) {
			for (uint32_t j = 0; j < seq2size; j++) {
				diffM[i][j] = fabs(seq1[i] - seq2[j]);
			}
		}


		// Create distance matrix (forward direction)

		double** pathM = new double*[seq1size];
		for (uint32_t i = 0; i < seq1size; i++) {
			pathM[i] = new  double[seq2size];
		}

		pathM[0][0] = diffM[0][0];
		for (uint32_t i = 1; i < seq1size; i++) {
			pathM[i][0] = diffM[i][0] + pathM[i - 1][0];
		}
		for (uint32_t j = 1; j < seq2size; j++) {
			pathM[0][j] = diffM[0][j] + pathM[0][j - 1];
		}

		for (uint32_t i = 1; i < seq1size; i++) {
			for (uint32_t j = 1; j < seq2size; j++) {

				if (pathM[i - 1][j - 1] < pathM[i - 1][j]) {

					if (pathM[i - 1][j - 1] < pathM[i][j - 1]) {
						pathM[i][j] = diffM[i][j] + pathM[i - 1][j - 1];
					} else {
						pathM[i][j] = diffM[i][j] + pathM[i][j - 1];
					}

				} else {

					if (pathM[i - 1][j] < pathM[i][j - 1]) {
						pathM[i][j] = diffM[i][j] + pathM[i - 1][j];
					} else {
						pathM[i][j] = diffM[i][j] + pathM[i][j - 1];
					}
				}

			}
		}


		// Find the warping path (backward direction)

		uint32_t warpSize = seq1size * seq2size;
		double* warpPath = new double[warpSize];

		uint32_t warpPathIndex = 0;
		uint32_t i = seq1size - 1, j = seq2size - 1;

		warpPath[warpPathIndex] = pathM[i][j];
		DEBUG("Path (%d, %d): %f", i, j, pathM[i][j]);

		do {
			if (i > 0 && j > 0) {

				if (pathM[i - 1][j - 1] < pathM[i - 1][j]) {
					if (pathM[i - 1][j - 1] < pathM[i][j - 1]) {
						i--; j--;
					} else {
						j--;
					}

				} else {
					if (pathM[i - 1][j] < pathM[i][j - 1]) {
						i--;
					} else {
						j--;
					}
				}

			} else {
				if (0 == i) {
					j--;
				} else {
					i--;
				}
			}

			warpPath[++warpPathIndex] = pathM[i][j];
			DEBUG("Path (%d, %d): %f", i, j, pathM[i][j]);

		} while (i > 0 || j > 0);


		// Calculate path measure

		double distance = 0.;
		for (uint32_t k = 0; k < warpPathIndex + 1; k++) {
			distance += warpPath[k];
		}
		distance = distance / (warpPathIndex + 1);


		// Clean up

		delete [] warpPath;

		for (uint32_t i = 0; i < seq1size; i++) {
			delete [] pathM[i];
		}
		delete [] pathM;

		for (uint32_t i = 0; i < seq1size; i++) {
			delete [] diffM[i];
		}
		delete [] diffM;

		return distance;
	}

} /* namespace math */
} /* namespace wtm */
