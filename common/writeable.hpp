#ifndef WRITEABLE_HPP
#define WRITEABLE_HPP

struct Writeable {
	virtual void write(FILE * desc) = 0;
};

#endif
