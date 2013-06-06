#ifndef TRIANGULATION_HPP
#define TRIANGULATION_HPP

#include <vector>

#include "../geometry/point.hpp"
#include "../geometry/triangle.hpp"
#include "../common/dumpable.hpp"
#include "figure_definition.hpp"

typedef std::vector<Point*>    PointsList;
typedef std::vector<Triangle*> TrianglesList;

struct Triangulation : public Dumpable {

	PointsList     points;
	TrianglesList  triangles;

	FigureDefinition * definition;

	Triangulation(FigureDefinition * figure) : definition(figure) {};

	void write(FILE * desc) {
		for(auto &triangle : triangles) {
			if(triangle)
				triangle->write(desc);
			else
				fprintf(desc, "[NULL Triangle]\n");
			fprintf(desc, "\n\n");
		}
	}

	Point *make_point(double x, double y) {
		Point *point;
		if (definition->parameter(x, y)) {
			point = new Point(x, y);
			points.push_back(point);
			return point;
		} else return NULL;
	}

	int check_triangle(const Point *p1, const Point *p2, const Point *p3) {
		return p1 && p2 && p3 &&
				definition->parameter(p1->x/4 + p2->x/4 + p3->x/2, p1->y/4 + p2->y/4 + p3->y/2) &&
        		definition->parameter(p1->x/4 + p3->x/4 + p2->x/2, p1->y/4 + p3->y/4 + p2->y/2);
	}

	int make_triangle(Point *p1, Point *p2, Point *p3, bool force = false) {
		Triangle *triangle;
		if (force || check_triangle(p1, p2, p3)) {
			triangle = new Triangle(p1, p2, p3);
			triangles.push_back(triangle);
			return 1;
		} else return 0;
	}

	void make_grid(int n, int m) {
		int i, j;
		Point * points[n][m];

		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				points[i][j] = make_point(i, j);
			}
		}

		for (i = 0; i < n - 1; i++) {
			for (j = 0; j < m - 1; j++) {
				(
					make_triangle(points[i][j + 1],     points[i][j],         points[i + 1][j + 1]) +
					make_triangle(points[i + 1][j],     points[i + 1][j + 1], points[i][j]        )
				) || (
					make_triangle(points[i][j],         points[i + 1][j],     points[i][j + 1]    ) +
					make_triangle(points[i + 1][j + 1], points[i][j + 1],     points[i + 1][j]    )
				);
			}
		}
	}

	void divide(Triangle *old) {
		Point *p1, *p2, *p3;

		p1 = make_point((old->p1->x + old->p2->x)/2, (old->p1->y + old->p2->y)/2);
		p2 = make_point((old->p2->x + old->p3->x)/2, (old->p2->y + old->p3->y)/2);
		p3 = make_point((old->p3->x + old->p1->x)/2, (old->p3->y + old->p1->y)/2);

		make_triangle(p1,      old->p1, p3,      true);
		make_triangle(p1,      p2,      old->p2, true);
		make_triangle(old->p3, p2,      p3,      true);
		
		old->p1 = p1;
		old->p2 = p2;
		old->p3 = p3;
	}

	void make_it_smaller(void) {
		// Reserve four-times space for triangle pointers at once
		// Required for not to invalidate the iterators while iterating
		triangles.reserve(triangles.size() * 4);

		for(auto& triangle : triangles)
			divide(triangle);
	}

	void destroy_points(void) {
		for(auto &point : points)
			delete point;
	}

	void destroy_triangles(void) {
		for(auto &triangle : triangles)
			delete triangle;
	}

	void destroy_all(void) {
		destroy_triangles();
		destroy_points();
	}
};

#endif
