#include "Vector.h"

Vector::Vector(size_t size) : _size(size)  {
	_data = new unsigned int[size];
}

Vector::~Vector() {
	delete[] _data;
}
