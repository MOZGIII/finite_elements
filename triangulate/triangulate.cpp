#include <cstdio>
#include <cstdlib>

#include "geometry/point.hpp"
#include "geometry/triangle.hpp"

#include "triangulation/figure_definition.hpp"
#include "triangulation/triangulation.hpp"

struct HUI : public FigureDefinition {
	int parameter(double x, double y) {
		//return !(y > x + 3 || y > 12 - x || (y > 2 && y < 4 && y < x && y < 9 - x));
		return
		(y<=1 && ((y<=x && y>=x-2) || (y>=4-x && y<=6-x))) ||
		(y>=1 && y<=2 && (y<=x && y<=6-x)) ||
		(y>=2 && y<=3 && (y>=4-x && y>=x-2)) ||
		(y>=3 && y<=4 && ((y<=x && y>=x-2) || (y>=4-x && y<=6-x))) ||
		
		(y<=2 && y<=x-6 && y>=x-8) ||
		(y>=2 && y<=3 && (y>=10-x && y>=x-8)) ||
		(y>=3 && y<=4 && ((y<=x-6 && y>=x-8) || (y>=10-x && y<=12-x))) ||
		
		(y<=4 && x>=12 && x<=14) ||
		(x>=14 && x<=16 && y>=x-14 && y<=x-12) ||
		(y<=4 && x>=16 && x<=18) ||
		
		(y>=19-x && y>=x-11);
	}
};


HUI hui;

// Triangulacia nahui
Triangulation triangulation(&hui);

void print(void) {
	FILE *gmain;

	gmain = fopen("main", "w");
	fprintf(gmain, "plot 'data' with lines\npause mouse close\n");
	fclose(gmain);

	triangulation.dump("data");

	system("gnuplot main");
	unlink("main");
	unlink("data");
}

void demo() {
	triangulation.make_grid(19, 6);
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
