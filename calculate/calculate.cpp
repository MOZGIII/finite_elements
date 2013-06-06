#include <cstdio>
#include <cstdlib>

#include "geometry/extended_point.hpp"
#include "geometry/extended_triangle.hpp"

#include "triangulation/figure_definition.hpp"
#include "triangulation/triangulation.hpp"

#include "calculation/matrix.hpp"
#include "calculation/calculation.hpp"

struct HUI : public FigureDefinition {
	int parameter(double x, double y) {
		return !(  9 - y < x - 3  );

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


struct Func : public FunctionDefinition {

	double T1 = 20, A = 1.5, alpha = 2.5, beta = 6;
	double call(double x, double y) {
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
