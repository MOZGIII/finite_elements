#ifndef CALCULATION_HPP
#define CALCULATION_HPP

#include <vector>
#include <map>

#include "function_definition.hpp"
#include "boundary_conditions_definition.hpp"
#include "matrix.hpp"
#include "../geometry/extended_point.hpp"
#include "../geometry/extended_triangle.hpp"
#include "../common/dumpable.hpp"

typedef std::vector<ExtendedPoint*>    ExtendedPointsList;
typedef std::vector<ExtendedTriangle*> ExtendedTrianglesList;

struct Calculation : public Dumpable {
	
	ExtendedPointsList     points;
	ExtendedTrianglesList  triangles;

	FunctionDefinition * function;
	BoundaryConditionsDefinition * boundary_conditions;

	Calculation(FunctionDefinition * _function) : function(_function) {};

	void write(FILE * desc) {
		for(auto &triangle : triangles) {
			if(triangle)
				triangle->write(desc);
			else
				fprintf(desc, "[NULL Triangle]\n");
			fprintf(desc, "\n\n");
		}
	}

	void import(Triangulation * triangulation) {
		std::map<void*, ExtendedPoint*> assoc;

		points.reserve(triangulation->points.size());
		for(auto &point : triangulation->points) {
			ExtendedPoint * extended_point = new ExtendedPoint(*point);
			{
				// Mutex lock;
				extended_point->index = points.size();
				points.push_back(extended_point);
			}
			assoc[point] = extended_point;
		}

		triangles.reserve(triangulation->triangles.size());
		for(auto &triangle : triangulation->triangles) {
			ExtendedTriangle * extended_triangle = new ExtendedTriangle(
					assoc[triangle->p1],
					assoc[triangle->p2],
					assoc[triangle->p3]
				);

			{
				// Mutex lock;
				extended_triangle->index = triangles.size();
				triangles.push_back(extended_triangle);
			}
		}
	}

	void init_boundary_conditions(void) {
		if(boundary_conditions) {
			for(auto &point : points) {
				point->u = boundary_conditions->fixed_value(point->x, point->y);
			}
		}
	}

	double get_function_value(Triangle * triangle) {
		Point c = triangle->center();
		double a = triangle->area();
		return function->call( c.x, c.y ) * a / 3;
	}

	void add_boundary_conditions(Triangle * triangle, Matrix & load) {
		if(boundary_conditions) {
			load.inc(0, 0, boundary_conditions->stream(triangle->p1->x, triangle->p1->y));
			load.inc(0, 1, boundary_conditions->stream(triangle->p2->x, triangle->p2->y));
			load.inc(0, 2, boundary_conditions->stream(triangle->p3->x, triangle->p3->y));
		}
	}

	Matrix get_load_vector(Triangle * triangle) {
		double func = get_function_value(triangle);
		Matrix load(1, 3);
		load.set(0, 0, func);
		load.set(0, 1, func);
		load.set(0, 2, func);
		add_boundary_conditions(triangle, load);

		if(boundary_conditions) {
			if(boundary_conditions->is_locked(triangle->p1->x, triangle->p1->y)) load.set(0, 0, 0);
			if(boundary_conditions->is_locked(triangle->p2->x, triangle->p2->y)) load.set(0, 1, 0);
			if(boundary_conditions->is_locked(triangle->p3->x, triangle->p3->y)) load.set(0, 2, 0);
		}

		return load;
	}

	Matrix & get_global_matrix(void) {
		int size = points.size();

		Matrix &K = *( new Matrix(size+1, size) );

		for (auto &triangle : triangles) {
			Matrix m = triangle->stiffness_matrix();
			Matrix l = get_load_vector(triangle);
			
			K.inc(triangle->p1->index, triangle->p1->index, m.get(0, 0));
			K.inc(triangle->p1->index, triangle->p2->index, m.get(0, 1));
			K.inc(triangle->p1->index, triangle->p3->index, m.get(0, 2));
			K.inc(triangle->p2->index, triangle->p1->index, m.get(1, 0));
			K.inc(triangle->p2->index, triangle->p2->index, m.get(1, 1));
			K.inc(triangle->p2->index, triangle->p3->index, m.get(1, 2));
			K.inc(triangle->p3->index, triangle->p1->index, m.get(2, 0));
			K.inc(triangle->p3->index, triangle->p2->index, m.get(2, 1));
			K.inc(triangle->p3->index, triangle->p3->index, m.get(2, 2));
			K.inc(triangle->p1->index, size, l.get(0, 0));
			K.inc(triangle->p2->index, size, l.get(0, 1));
			K.inc(triangle->p3->index, size, l.get(0, 2));
		}
		return K;
	}

	void transform_global_matrix(Matrix &K) {
		int i, n;

		int size = points.size();

		for (n = 0; n < size; n++) {
			if (boundary_conditions && boundary_conditions->is_locked(points[n]->x, points[n]->y)) {

				for ( i = 0; i < size; i++) {
					if (i != n) K.set(n, i, 0);
				}

				K.set(n, size, K.get(n, n) * points[n]->u);

				for (i = 0; i < size; i++) {
					if (i != n) {
						K.dec(i, size, K.get(i, n) * points[n]->u);
						K.set(i, n, 0);
					}
				}

			}
		}
	}

	void gauss(Matrix &K) {
		double k_ii_i, k_i_i;
		int i, j, ii, jj;
		
		int n_nodes = points.size();

		for ( i = 0; i < n_nodes; i++) {
			k_i_i = K.get(i, i);
			for ( j = 0; j < n_nodes + 1; j++) {
				K.set(i, j, K.get(i, j) / k_i_i);
			}

			for ( ii = i + 1; ii < n_nodes; ii++) {
				if (K.get(ii, i) != 0) {
					k_ii_i = K.get(ii, i);
					for ( jj = i; jj < n_nodes + 1; jj++) {
						K.dec(ii, jj, k_ii_i * K.get(i, jj));
					}
				}
			}
		}

		for ( i = n_nodes - 1; i >= 0; i--) {
			for ( ii = i - 1; ii >= 0; ii--) {
				k_ii_i = K.get(ii, i);
				for ( jj = i; jj < n_nodes + 1; jj++) {
					K.dec(ii, jj, K.get(i, jj) * k_ii_i);
				}
			}
		}

		for ( i = 0; i < n_nodes; i++) {
			points[i]->u = K.get(i, n_nodes);
		}
	}

	void calculate(void) {
		Matrix &K = get_global_matrix();
		transform_global_matrix(K);
		gauss(K);
	}
};

#endif
