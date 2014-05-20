#include <assert.h>
#include <DTW.h>

namespace wtm {
namespace math {

	enum DIRECTION {RIGHT, DOWN, DIAG};

	double DTW::calcDistance(double* seq1, uint32_t seq1size, double* seq2, uint32_t seq2size) {

		// Create distances matrix
		double** distanceMatrix = new double*[seq1size];
		for (uint32_t i = 0; i < seq1size; i++) {
			distanceMatrix[i] = new  double[seq2size];
		}

		for (uint32_t i = 0; i < seq1size; i++) {
			for (uint32_t j = 0; j < seq2size; j++) {
				distanceMatrix[i][j] = fabs(seq1[i] - seq2[j]);
			}
		}

		// Find the warping path
		uint32_t warpSize = seq1size * seq2size;
		double* warpPath = new double[warpSize];

		uint32_t warpPathIndex = 0;
		warpPath[warpPathIndex] = distanceMatrix[0][0];

		uint32_t i = 0, j = 0;
		double varaints[3];
		while (i < seq1size - 1 || j < seq2size - 1) {
			DEBUG("Index (%d, %d), warp %d", i, j, warpPathIndex);

			// Find possible variants to go
			varaints[RIGHT] = -1;
			varaints[DOWN] = -1;
			varaints[DIAG] = -1;

			if (i < seq1size - 1) {
				varaints[RIGHT] = distanceMatrix[i + 1][j];
			}
			if (j < seq2size - 1) {
				varaints[DOWN] = distanceMatrix[i][j + 1];
			}
			if (i < seq1size - 1 && j < seq2size - 1) {
				varaints[DIAG] = distanceMatrix[i + 1][j + 1];
			}

			// Find the shortest way
			double minDist = -1;
			int minPosition = -1;
			for (int k = 0; k < 3; k++) {

				if (varaints[k] >= 0) {
					if (minDist >= 0) {

						if (varaints[k] < minDist) {
							minDist = varaints[k];
							minPosition = k;
						}

					} else {
						minDist = varaints[k];
						minPosition = k;
					}
				}
			}

			// Increase the index related with chosen direction
			if (RIGHT == minPosition) {
				i++;

			} else if (DOWN == minPosition) {
				j++;

			} else if (DIAG == minPosition) {
				i++, j++;
			}

			if (minDist >= 0) {
				DEBUG("Value %f (total %f)", minDist, warpPath[warpPathIndex] + minDist);

				warpPath[warpPathIndex + 1] = warpPath[warpPathIndex] + minDist;
				warpPathIndex++;

			} else {
				assert(minDist < 0 && "Invalid distance");
			}
		}

		// Calculate path measure
		double distance = 0.;
		for (uint32_t k = 0; k < warpPathIndex - 1; k++) {
			distance += warpPath[k];
		}
		distance = sqrt(distance);

		// Clean up
		delete [] warpPath;

		for (uint32_t i = 0; i < seq1size; i++) {
			delete [] distanceMatrix[i];
		}
		delete [] distanceMatrix;

		return distance;
	}

} /* namespace math */
} /* namespace wtm */
