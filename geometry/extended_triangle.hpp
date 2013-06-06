#ifndef EXTENDED_TRIANGLE_HPP
#define EXTENDED_TRIANGLE_HPP

#include "triangle.hpp"
#include "extended_point.hpp"
#include "../calculation/matrix.hpp"

struct ExtendedTriangle : public Triangle {
	int index;

	ExtendedPoint *p1;
	ExtendedPoint *p2;
	ExtendedPoint *p3;

	ExtendedTriangle(ExtendedPoint * _p1, ExtendedPoint * _p2, ExtendedPoint * _p3) : Triangle(_p1, _p2, _p3), index(-1), p1(_p1), p2(_p2), p3(_p3) {};

	Matrix stiffness_matrix(void) {
		double a = area();

		double bi = p2->y - p3->y;
		double bj = p3->y - p1->y;
		double bk = p1->y - p2->y;
		double ci = p3->x - p2->x;
		double cj = p1->x - p3->x;
		double ck = p2->x - p1->x;

		Matrix matrix(3, 3);
		matrix.set(0, 0, (bi * bi + ci * ci) / (4*a) + a / 6);
		matrix.set(0, 1, (bi * bj + ci * cj) / (4*a) + a / 12);
		matrix.set(0, 2, (bi * bk + ci * ck) / (4*a) + a / 12);
		matrix.set(1, 0, (bj * bi + cj * ci) / (4*a) + a / 12);
		matrix.set(1, 1, (bj * bj + cj * cj) / (4*a) + a / 6);
		matrix.set(1, 2, (bj * bk + cj * ck) / (4*a) + a / 12);
		matrix.set(2, 0, (bk * bi + ck * ci) / (4*a) + a / 12);
		matrix.set(2, 1, (bk * bj + ck * cj) / (4*a) + a / 12);
		matrix.set(2, 2, (bk * bk + ck * ck) / (4*a) + a / 6);

		return matrix;
	} 
};

#endif
