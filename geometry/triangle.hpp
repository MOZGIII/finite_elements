#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "point.hpp"
#include "../common/writeable.hpp"

struct Triangle : public Writeable {
	Point *p1;
	Point *p2;
	Point *p3;

	Triangle(Point * _p1, Point * _p2, Point * _p3) : p1(_p1), p2(_p2), p3(_p3) {};
	virtual ~Triangle() {};

	void write(FILE * desc) {
		#define check_write(ptr) { if( (ptr) ) (ptr)->write(desc); else fprintf(desc, "%s\n", "[NULL Point]"); }

		check_write(p1);
		check_write(p2);
		check_write(p3);
		check_write(p1);
	}

	Point center() {
		return Point(
				(p1->x + p2->x + p3->x) / 3,
				(p1->y + p2->y + p3->y) / 3
			);
	}

	double signed_area() {
		return ((p2->x - p1->x) * (p3->y - p1->y) - (p3->x - p1->x) * (p2->y - p1->y)) / 2;
	}

	double area() {
		double value = signed_area();
		return value > 0 ? value : -value;
	}
};

#endif
