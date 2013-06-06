#ifndef BOUNDARY_CONDITIONS_DEFINITION_HPP
#define BOUNDARY_CONDITIONS_DEFINITION_HPP


// This is an abstract class, you have to inherit it.
struct BoundaryConditionsDefinition {

	// Implement the function to apply streaming boundaries.
	virtual double stream(double x, double y) = 0;

	// Implement the function to apply fixed boundaries.
	virtual double fixed_value(double x, double y) = 0;

	// Implement the function to lock changes in certain points.
	virtual bool is_locked(double x, double y) = 0;

};

#endif
