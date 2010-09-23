#include "Vector.h"
#include <stdexcept>

#define INIT_DATA(data,size) data = new unsigned int[size];

Vector::Vector(size_t size) : _size(size)  {
	INIT_DATA(_data,size);
	memset(_data,0,sizeof(unsigned int)*size);
}

Vector::Vector(const Vector &vector) : _size(vector._size) {
	INIT_DATA(_data,vector._size);
	memcpy(_data, vector._data, sizeof(unsigned int)*_size);
}

Vector::~Vector() {
	delete[] _data;
}

Vector& Vector::operator=(const Vector &vector) {
	unsigned int * tmp;
	INIT_DATA(tmp,vector._size);
	memcpy(_data, vector._data, sizeof(unsigned int)*vector._size);
	delete[] _data;
	_data=tmp;
	_size = vector._size;
	
	return *this;
}

unsigned int& Vector::operator[](unsigned int index) const {
	if (index < _size)
		return _data[index];
	else
		throw std::out_of_range("index");
}

