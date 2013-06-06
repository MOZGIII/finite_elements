#include <cstdio>
#include <cstdlib>

#include "geometry/extended_point.hpp"
#include "geometry/extended_triangle.hpp"

#include "triangulation/figure_definition.hpp"
#include "triangulation/triangulation.hpp"

#include "calculation/matrix.hpp"
#include "calculation/calculation.hpp"

struct Task : public FigureDefinition {
	int parameter(double x, double y) {
		return !(  9 - y < x - 3  );
	}
};

Task task;
Triangulation triangulation(&task);


struct Func : public FunctionDefinition {

	double call(double x, double y) {
		double T1 = 20, A = 1.5, alpha = 2.5, beta = 6;
		// return A * exp( -beta  ) *  cos( sqrt(x*x+y*y ));
		//return A*sin(x*y);
		return x;
	}

};

Func func;
Calculation calculation(&func);


void print(void) {
	FILE *gmain;

	gmain = fopen("main", "w");
	fprintf(gmain, "set key off\nset view map\nsplot 'data' with lines palette\npause mouse close\n");
	fclose(gmain);

	calculation.dump("data");

	system("gnuplot main");
	unlink("main");
	//unlink("data");
}

int main() {
	triangulation.make_grid(10, 10);
	triangulation.make_it_smaller();
	triangulation.make_it_smaller();


	calculation.import(&triangulation);
	// print();
	calculation.calculate();
	print();

	return 0;
}
