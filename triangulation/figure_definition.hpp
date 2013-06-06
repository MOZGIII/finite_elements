#ifndef FIGURE_DEFINITION_HPP
#define FIGURE_DEFINITION_HPP


// This is an abstract class, you have to inherit it.
struct FigureDefinition {
	
	// Return 1 where the point should be and 0 where it should not be. EZ.
	virtual bool parameter(double x, double y) = 0;
};

#endif
