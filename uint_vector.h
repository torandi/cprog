#ifndef _VECTOR_UINT_
#define _VECTOR_UINT_
#include <cstring>

class Vector {
	unsigned int * _data;
	size_t _size;
	
public:
	Vector();
	explicit Vector(size_t size);
	Vector(const Vector &vector);
	~Vector();
	Vector& operator=(const Vector &vector);
	unsigned int& operator[](size_t index) const;
};
#endif