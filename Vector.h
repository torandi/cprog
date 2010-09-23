#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <cstring>

class Vector {
	unsigned int * _data;
	size_t _size;

public:
	explicit Vector(size_t size);
	explicit Vector(const Vector &vector);
	~Vector();
	Vector& operator=(const Vector &vector);
	unsigned int& operator[](unsigned int index) const;
};

#endif
