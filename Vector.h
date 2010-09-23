#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <cstring>

class Vector {
	unsigned int * _data;
	size_t _size;

public:
	Vector(size_t size);
	Vector(const Vector &vector);
	~Vector();
};

#endif
