#ifndef PRINTER_H_
#define PRINTER_H_

#include "../config.h"
#include <vector>
#include <string>

using namespace std;

namespace yazz {
namespace math {

/**
 * Helper for printing data
 */
class Printer {
public:

	static void printVector(char* vector, size_t size);
	static void printVector(double* vector, size_t size);
	static void printVector(const vector<string>& vector, size_t size);
	static void printMatrix(double** matrix, size_t sizeX, size_t sizeY);
	static void printMatrix3D(double*** matrix, size_t sizeX, size_t sizeY, size_t sizeZ);
};

} /* namespace math */
} /* namespace yazz */

#endif /* PRINTER_H_ */
