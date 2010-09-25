#include "Vector.h"
#include <stdexcept>

#define INIT_DATA(data,size) data = new T*[size];

template<class T>
Vector<T>::Vector(size_t size) : _size(size)  {
	INIT_DATA(_data,size);
	_elements=0;
	memset(_data,0,sizeof(T*)*size);
}

template<class T>
Vector<T>::Vector(const Vector<T> &vector) : _size(vector._size) {
	INIT_DATA(_data,vector._size);
	memcpy(_data, vector._data, sizeof(T*)*_size);
}

template<class T>
Vector<T>::~Vector() {
	delete[] _data;
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T> &vector) {
	T** tmp;
	INIT_DATA(tmp,vector._size);
	memcpy(_data, vector._data, sizeof(T*)*vector._size);
	delete[] _data;
	_data=tmp;
	_size = vector._size;
	
	return *this;
}

template<class T>
T& Vector<T>::operator[](unsigned int index) const {
	if (index < _size)
		return _data[index];
	else
		throw std::out_of_range("index");
}

template<class T>
void Vector<T>::push_back(const T &t) {
	if (_elements >= _size) {
		resize();
	}
	_data[_elements] = t;
	_elements++;
	return;
}

template<class T>
void Vector<T>::resize() {
	T** tmp;
	INIT_DATA(tmp,(vector._size + 100));
	memcpy(_tmp,_data,sizeof(T*)*_size);
	delete[] _data;
	_data = tmp;
	return;
}
