#include <Printer.h>

namespace yazz {
namespace math {

void Printer::printVector(double* vector, size_t size) {
	cout << "[";

	for (size_t i = 0; i < size; i++) {
		cout << vector[i];

		if (i < size - 1) {
			cout << ", ";
		}
	}

	cout << "]";
}

void Printer::printVector(char* vector, size_t size) {
	cout << "[";

	for (size_t i = 0; i < size; i++) {
		cout << vector[i];

		if (i < size - 1) {
			cout << ", ";
		}
	}

	cout << "]";
}

void Printer::printMatrix(double** matrix, size_t sizeX, size_t sizeY) {
	for (size_t i = 0; i < sizeX; i++) {
		for (size_t j = 0; j < sizeY; j++) {

			cout << matrix[i][j];

			if (i < sizeY - 1) {
				cout << ",\t";
			}
		}
		cout << endl;
	}
}

void Printer::printMatrix3D(double*** matrix, size_t sizeX, size_t sizeY, size_t sizeZ) {
	for (size_t i = 0; i < sizeX; i++) {
		for (size_t j = 0; j < sizeY; j++) {
			cout << "\t";

			for (size_t l = 0; l < sizeZ; l++) {
				cout << matrix[i][j][l];

				if (l < sizeZ - 1) {
					cout << ",\t";
				}
				cout << endl;
			}
		}
		cout << endl;
	}
}

} /* namespace math */
} /* namespace yazz */
