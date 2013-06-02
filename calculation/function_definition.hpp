#ifndef FUNCTION_DEFINITION_HPP
#define FUNCTION_DEFINITION_HPP


// This is an abstract class, you have to inherit it.
struct FunctionDefinition {

	// Implement the function to integrate with.
	virtual double call(double x, double y) = 0;
};

#endif
