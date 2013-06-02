#ifndef POINT_HPP
#define POINT_HPP

#include "../common/writeable.hpp"

struct Point : public Writeable {
	double x;
	double y;

	Point(double _x, double _y) : x(_x), y(_y) {}
	virtual ~Point() {};

	void write(FILE * desc) {
		fprintf(desc, "%lf %lf\n", x, y);
	}
};

#endif
