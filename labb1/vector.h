#ifndef _VECTOR_MIKTOR_H_
#define _VECTOR_MIKTOR_H_
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#define DEFAULT_SIZE 100
#define RESIZE_SIZE 100

template <class T>
class Vector {
	T * _data;
	size_t _size;
	size_t _elements;

	void resize();
	
public:
	Vector();
	explicit Vector(size_t size);
	Vector(const Vector<T> &vector);
	Vector(const size_t &size, T default_value);
	~Vector();
	Vector<T>& operator=(const Vector<T> &vector);
	T& operator[](size_t index) const;
	void push_back(const T& t);
	void insert(size_t index, const T& t);
	void erase(size_t index);
	void clear();
	size_t size() const;
	void sort(bool ascending = true);
	void printVector();

};

#define INIT_DATA(data,size) data = new T[size];

template<class T>
Vector<T>::Vector() : _size(DEFAULT_SIZE) {
	INIT_DATA(_data,DEFAULT_SIZE);
	_elements=0;
}

template<class T>
Vector<T>::Vector(size_t size) : _size(size)  {
	INIT_DATA(_data,size);
	for(size_t i=0;i<size;++i) {
		_data[i] = T();
	}
	_elements=_size;
}

template<class T>
Vector<T>::Vector(const Vector<T> &vector) : _size(vector._size) {
	INIT_DATA(_data,vector._size);
	_elements=vector._elements;
	for(size_t i=0;i<_size;++i) {
		_data[i]=vector._data[i];
	}
}

template<class T>
Vector<T>::Vector(const size_t &size, T default_value) : _size(size) {
	INIT_DATA(_data,size);
	for(size_t i = 0; i < size; ++i) {
		_data[i] = T(default_value);
	}
	_elements = size;
}

template<class T>
Vector<T>::~Vector() {
	delete[] _data;
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T> &vector) {
	T* tmp;
	INIT_DATA(tmp,vector._size);
	for(size_t i=0;i<vector._elements;++i) {
		tmp[i]=vector._data[i];
	}
	delete[] _data;
	_data=tmp;
	_size = vector._size;
	_elements= vector._elements;
	
	return *this;
}

template<class T>
T& Vector<T>::operator[](size_t index) const {
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
	_data[_elements++] = t;
}

template<class T>
void Vector<T>::resize() {
	T* tmp;
	INIT_DATA(tmp,(_size+RESIZE_SIZE));
	for(size_t i=0;i<_size;++i) {
		tmp[i]=_data[i];
	}
	_size=_size+RESIZE_SIZE;
	delete[] _data;
	_data = tmp;
}

template<class T>
void Vector<T>::erase(size_t index) {
	if (index < _elements) {
		for(size_t i=index;i<_elements-1;++i) {
			_data[i]=_data[i+1];
		}
		--_elements;
	} else {
		throw std::out_of_range("index");
	}
}

template<class T>
void Vector<T>::clear() {
	delete[] _data;
	INIT_DATA(_data,DEFAULT_SIZE);
	_elements = 0;
}

template<class T>
void Vector<T>::insert(size_t index,const T& t) {
	if(index==_elements) {
		push_back(t);
	} else if(index<_elements){
		if(_elements>=_size) {
			resize();
		}
		for(size_t i=_elements;i>index;--i) {
			_data[i]=_data[i-1];
		}
		_data[index]=t;
		++_elements;
	} else {
		throw std::out_of_range("index");
	}
}

template<class T>
void Vector<T>::sort(bool ascending) {
	// For now, only sort ascending.
	//quicksort(_data, 0, _elements, ascending);
	//if (ascending) {
	std::sort(_data, _data + _elements);
	//} else {
	//	std::sort(_data,_data + _elements, std::greater<T>());
	//}
	if (!ascending) {
		std::reverse(_data, _data+_elements);
	}
}

template <class T>
void Vector<T>::printVector() {
	int i = 0;
	for(i = 0; i < (int) _elements; ++i) {
		std::cout << _data[i] << ' ';
	}
	
	std::cout << std::endl;
}

#endif
