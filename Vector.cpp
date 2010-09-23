#include "Vector.h"
#include <cstring>

Vector::Vector(size_t size) : _size(size)  {
	_data = new unsigned int[size];
	memset(_data,0,sizeof(unsigned int)*size);
}

Vector::Vector(const Vector &vector) : _size(vector._size) {
    _data = new unsigned int[vector._size];
    memcpy(_data, vector._data, sizeof(unsigned int)*_size);
}

Vector::~Vector() {
	delete[] _data;
}
