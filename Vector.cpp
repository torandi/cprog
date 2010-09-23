#include "Vector.h"
#include <stdexcept>

#define INIT_DATA(size) _data = new unsigned int[size];

Vector::Vector(size_t size) : _size(size)  {
	INIT_DATA(size);
	memset(_data,0,sizeof(unsigned int)*size);
}

Vector::Vector(const Vector &vector) : _size(vector._size) {
	INIT_DATA(vector._size);
	memcpy(_data, vector._data, sizeof(unsigned int)*_size);
}

Vector::~Vector() {
	delete[] _data;
}

Vector& Vector::operator=(const Vector &vector) {
	delete[] _data;
	INIT_DATA(vector._size);
	memcpy(_data, vector._data, sizeof(unsigned int)*vector._size);
	_size = vector._size;
	
	return *this;
}

unsigned int& Vector::operator[](unsigned int index) const {
	if (index < _size)
		return _data[index];
	else
		throw std::out_of_range("index");
}

