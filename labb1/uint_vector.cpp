#include "uint_vector.h"
#include <stdexcept>
#define DEFAULT_SIZE 100
#define INIT_DATA(data,size) data = new unsigned int[size];

Vector::Vector() : _size(DEFAULT_SIZE) {
	INIT_DATA(_data,DEFAULT_SIZE);
}

Vector::Vector(size_t size) : _size(size) {
	INIT_DATA(_data, size);
	for(size_t i=0;i<size;++i) {
		_data[i] = 0;
	}
}

Vector::Vector(const Vector &vector) : _size(vector._size) {
	INIT_DATA(_data,vector._size);
	for(size_t i=0;i<_size;++i) {
		_data[i]=vector._data[i];
	}
}

Vector::~Vector() {
	delete[] _data;
}

Vector& Vector::operator=(const Vector &vector) {
	unsigned int* tmp;
	INIT_DATA(tmp,vector._size);
	for(size_t i=0;i<vector._size;++i) {
		tmp[i]=vector._data[i];
	}
	delete[] _data;
	_data=tmp;
	_size = vector._size;
	
	return *this;
}

unsigned int& Vector::operator[](size_t index) const {
	if (index < _size)
		return _data[index];
	else
		throw std::out_of_range("index");
}