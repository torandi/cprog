#ifndef _VECTOR_MIKTOR_H_
#define _VECTOR_MIKTOR_H_
#include <cstring>
#include <stdexcept>

#define DEFAULT_SIZE 100

template <class T>
class Vector {
	T** _data;
	size_t _size;
	size_t _elements;

	void resize();
	void delete_data();

public:
	Vector();
	explicit Vector(size_t size);
	Vector(const Vector<T> &vector);
	~Vector();
	Vector<T>& operator=(const Vector<T> &vector);
	T& operator[](unsigned int index) const;
	void push_back(const T& t);
	void insert(size_t i, const T& t);
	const T& erase(size_t i);
	void clear();
	size_t size() const;
	void sort(bool ascending = true);

};

#define INIT_DATA(data,size) data = new T*[size];

template<class T>
Vector<T>::Vector() : _size(DEFAULT_SIZE)  {
	INIT_DATA(_data,DEFAULT_SIZE);
	_elements=0;
}

template<class T>
Vector<T>::Vector(size_t size) : _size(size)  {
	INIT_DATA(_data,size);
	_elements=_size;
	for(size_t i=0;i<_size;++i) {
		_data[i]=new T();
	}
}

template<class T>
Vector<T>::Vector(const Vector<T> &vector) : _size(vector._size) {
	INIT_DATA(_data,vector._size);
	_elements=_size;
	for(size_t i=0;i<_size;++i) {
		_data[i]=vector._data[i];
	}
}

template<class T>
Vector<T>::~Vector() {
	delete_data();
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T> &vector) {
	T** tmp;
	INIT_DATA(tmp,vector._size);
	for(int i=0;i<vector._elements;++i) {
		tmp[i]=vector._data[i];
	}
	delete_data();
	_data=tmp;
	_size = vector._size;
	_elements= vector._elements;
	
	return *this;
}

template<class T>
T& Vector<T>::operator[](unsigned int index) const {
	if (index < _elements)
		return *_data[index];
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
	_data[_elements++] = new T(t);
}

template<class T>
void Vector<T>::resize() {
	T** tmp;
	INIT_DATA(tmp,(_size*2));
	for(size_t i=0;i<_size;++i) {
		tmp[i]=_data[i];
	}
	delete_data();
	_data = tmp;
}

template<class T>
const T& Vector<T>::erase(size_t i) {
	/*
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
	*/
	return *_data[0];
}

template<class T>
void Vector<T>::delete_data() {
	for(size_t i=0;i<_elements;++i) {
		delete _data[i];
	}
	delete[] _data;
}

#endif
