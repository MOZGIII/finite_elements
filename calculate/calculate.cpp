#include <cstdio>
#include <cstdlib>

#include "geometry/extended_point.hpp"
#include "geometry/extended_triangle.hpp"

#include "triangulation/figure_definition.hpp"
#include "triangulation/triangulation.hpp"

#include "calculation/matrix.hpp"
#include "calculation/calculation.hpp"

struct Task : public FigureDefinition {
	bool parameter(double x, double y) {
		return !(  9 - y < x - 3  );
	}
};

Task task;
Triangulation triangulation(&task);


struct Func : public FunctionDefinition {

	double call(double x, double y) {
		double A = 1.5, alpha = 0.15, beta = 1;
		return A * exp( -alpha * x ) *  cos( beta * x );
	}

};


struct Bounds : public BoundaryConditionsDefinition {

	double stream(double x, double y) {
		if(x == 0) // g2
			return 0.15; // p1

		if (x == 9) // g4
			return -0.1; // p2

		return 0;
	}

	double fixed_value(double x, double y) {
		if(y == 0) // g1
			return 2; // u1

		if(y == 9 || (9 - y == x - 3)) // g3
			return 1.5; // u2

		return 0;
	}
	
	bool is_locked(double x, double y) {
		if(y == 0) // g1
			return true;

		if(y == 9 || (9 - y == x - 3)) // g3
			return true;

		return false;
	}

};

Func func;
Bounds bounds;
Calculation calculation(&func);

void print(void) {
	FILE *gmain;

	gmain = fopen("main", "w");
	fprintf(gmain, "set terminal wxt size 600,600\nset key off\nsplot 'data' with lines palette\npause mouse close\n");
	fclose(gmain);

	calculation.dump("data");

	system("gnuplot main");
	unlink("main");
	//unlink("data");
}

int main() {
	triangulation.make_grid(10, 10);
	// for(int i = 3; i > 0; i--)
		triangulation.make_it_smaller();

	calculation.boundary_conditions = &bounds;
	calculation.import(&triangulation);
	calculation.init_boundary_conditions();
	// print();
	calculation.calculate();
	print();

	return 0;
}
