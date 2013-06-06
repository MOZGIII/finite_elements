#include <cstdio>
#include <cstdlib>

#include <unistd.h>

#include "geometry/point.hpp"
#include "geometry/triangle.hpp"

#include "triangulation/figure_definition.hpp"
#include "triangulation/triangulation.hpp"

struct Task : public FigureDefinition {
	bool parameter(double x, double y) {
		return !(  9 - y < x - 3  );
	}
};


Task task;
Triangulation triangulation(&task);

void print(void) {
	FILE *gmain;

	gmain = fopen("main", "w");
	fprintf(gmain, "set key off\nplot 'data' with lines\npause mouse close\n");
	fclose(gmain);

	triangulation.dump("data");

	system("gnuplot main");
	unlink("main");
	unlink("data");
}

void demo() {
	triangulation.make_grid(10, 10);
	print();
	triangulation.make_it_smaller();
	print();
	triangulation.make_it_smaller();
	print();
}

void run() {
	triangulation.make_grid(19, 6);
	triangulation.make_it_smaller();
	triangulation.make_it_smaller();
	triangulation.write(stdout);
}

int main() {
	demo();
	return 0;
}
