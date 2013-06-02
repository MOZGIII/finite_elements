#ifndef DUMPABLE_HPP
#define DUMPABLE_HPP

#include "writeable.hpp"

struct Dumpable : public Writeable {
	void dump(const char * filename) {
		FILE * file;
		file = fopen(filename, "w");
		write(file);
		fclose(file);
	}
};

#endif
