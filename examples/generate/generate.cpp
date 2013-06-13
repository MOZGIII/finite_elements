#include <cstdio>
#include <cstdlib>

#include "triangulation/triangulation.hpp"
#include "calculation/calculation.hpp"

struct Task : public FigureDefinition, public BoundaryConditionsDefinition, public FunctionDefinition {
	int x_max;
	int y_max;
	
	Task(void) {
		x_max = 9;
		y_max = 9;
	}

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
		// Lock if non-zero initially
		return fixed_value(x, y) != 0;
	}

	// Function definition
	double call(double x, double y) {
		double A = 1.5, alpha = 0.15, beta = 1;
		return A * exp( -alpha * x ) *  cos( beta * x );
	}

} task;

void print(Dumpable * d, const char * filename) {
	FILE *gmain;
	int gnuplot_padding_x, gnuplot_padding_y;
	bool is3d = typeid(Triangulation) != typeid(*d);

	const char * drawproc;
	if(is3d) {
		gnuplot_padding_x = 77+76;
		gnuplot_padding_y = 78+97;
		drawproc = "set view map\nsplot 'data' with lines palette lw 2.5\n";
	} else {
		gnuplot_padding_x = 55;
		gnuplot_padding_y = 40;
		drawproc = "unset ytics\nunset xtics\nplot 'data' with lines lw 1\n";
	}

	gmain = fopen("main", "w");
	fprintf(gmain, "set terminal png size %d,%d\nset output \"%s\"\nset key off\n%s", 
		50 * task.x_max + gnuplot_padding_x, 50 * task.y_max + gnuplot_padding_y, filename, drawproc);
	fclose(gmain);

	d->dump("data");

	system("gnuplot main");
	unlink("main");
	//unlink("data");
}

void calculate_and_print(Triangulation &t, const char * filename) {
	Calculation calculation(&task);
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
