#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <valarray>

#include "../common/dumpable.hpp"

struct Matrix {

	int cols;
	int rows;

	std::valarray<double> data;

	Matrix(int _cols, int _rows) : cols(_cols), rows(_rows), data(rows * cols) {};

	double * p(int i, int j) {
		return &data[ i * cols + j ];
	}

	double get(int i, int j) {
		return *p(i, j);
	}

	void set(int i, int j, double value) {
		*p(i, j) = value;
	}

	void inc(int i, int j, double value) {
		*p(i, j) += value;
	}

	void dec(int i, int j, double value) {
		*p(i, j) -= value;
	}

	void write(FILE * desc) {
		for(int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				fprintf(desc, "%f ", get(i, j));
			}
			fprintf(desc, "\n");
		}
	}
};

#endif
