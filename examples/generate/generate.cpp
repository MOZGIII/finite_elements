#include <cstdio>
#include <cstdlib>

#include "triangulation/triangulation.hpp"
#include "calculation/calculation.hpp"

struct Task : public FigureDefinition, public BoundaryConditionsDefinition {
	int x_max = 9;
	int y_max = 9;

	// Figure definition
	bool parameter(double x, double y) {
		return !(  9 - y < x - 3  );
	}

	// Boundary conditions
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

} task;

struct Func : public FunctionDefinition {

	double call(double x, double y) {
		double A = 1.5, alpha = 0.15, beta = 1;
		return A * exp( -alpha * x ) *  cos( beta * x );
	}

} func;

void print(Dumpable * d, const char * filename) {
	FILE *gmain;
	bool is3d = typeid(Triangulation) != typeid(*d);

	const char * drawproc;
	if(is3d) {
		drawproc = "set view map\nsplot 'data' with lines palette\n";
	} else{
		drawproc = "plot 'data' with lines\n";
	}

	gmain = fopen("main", "w");
	fprintf(gmain, "set terminal png size %d,%d\nset output \"%s\"\nset key off\n%s", 50 * task.x_max, 50 * task.y_max, filename, drawproc);
	fclose(gmain);

	d->dump("data");

	system("gnuplot main");
	unlink("main");
	//unlink("data");
}

void calculate_and_print(Triangulation &t, const char * filename) {
	Calculation calculation(&func);
	calculation.boundary_conditions = &task;
	calculation.import(&t);
	calculation.init_boundary_conditions();
	calculation.calculate();
	print(&calculation, filename);
}

int main() {
	Triangulation triangulation(&task);

	triangulation.make_grid(task.x_max + 1, task.y_max + 1);
	print(&triangulation, "triangulation.png");
	calculate_and_print(triangulation, "calculation.png");

	for(int i = 0; i < 2; i++) {
		triangulation.make_it_smaller();
		char filename[256];

		snprintf(filename, 255, "triangulation_%d.png", i);
		print(&triangulation, filename);

		snprintf(filename, 255, "calculation_%d.png", i);
		calculate_and_print(triangulation, filename);
	}

	return 0;
}
