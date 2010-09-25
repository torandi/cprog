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
	if (index < _elements)
		return _data[index];
	else
		throw std::out_of_range("index");
}

template<class T>
size_t Vector<T>::size() const {
	return _elements;
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
	INIT_DATA(tmp,(_size + 100));
	memcpy(tmp,_data,sizeof(T*)*_size);
	delete[] _data;
	_data = tmp;
	return;
}

template<class T>
const T& Vector<T>::erase(size_t i) {
	T** tmp;
	INIT_DATA(tmp,(_size - 1));
	T t = _data[i]; // Element to be removed.
	// Copy values from 0 to (i-1);
	memcpy(tmp,_data,sizeof(T*)*i);
	// Should be size - (i-1) elements remaining.
	memcpy(tmp,_data,sizeof(T*)*(_size-(i-1)));
	// Delete old data.
	delete[] _data;
	_data = tmp;
	_size--;
	_elements--;
	return t;
}