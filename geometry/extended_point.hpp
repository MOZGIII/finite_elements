#ifndef EXTENDED_POINT_HPP
#define EXTENDED_POINT_HPP

#include "point.hpp"

#define POINT_NO_VALUE (-1)

struct ExtendedPoint : public Point {
	double u;
	int index;

	ExtendedPoint(double _x, double _y, double _u = POINT_NO_VALUE, int _index = -1) : Point(_x, _y), u(_u), index(_index) {}
	ExtendedPoint(Point &point, double _u = POINT_NO_VALUE, int _index = -1) : Point(point), u(_u), index(_index) {}

	void write(FILE * desc) {
		fprintf(desc, "%lf %lf %lf\n", x, y, u);
	}
};

#endif
