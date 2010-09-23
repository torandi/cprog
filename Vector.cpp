#include "Vector.h"

#define INIT_DATA(size) _data = new unsigned int[size];

Vector::Vector(size_t size) : _size(size)  {
	INIT_DATA(size);
	memset(_data,0,sizeof(unsigned int)*size);
}

Vector::Vector(const Vector &vector) : _size(vector._size) {
	INIT_DATA(size);
   memcpy(_data, vector._data, sizeof(unsigned int)*_size);
}

Vector::~Vector() {
	delete[] _data;
}
