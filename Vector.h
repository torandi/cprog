#ifndef _VECTOR_MIKTOR_H_
#define _VECTOR_MIKTOR_H_
#include <cstring>
#include <stdexcept>
#include <iostream>

#define DEFAULT_SIZE 100

template <class T>
class Vector {
	T** _data;
	size_t _size;
	size_t _elements;

	void resize();
	void delete_data();
	void swap(T *a, T *b);
	void quicksort(T** data, int l, int r);

public:
	Vector();
	explicit Vector(size_t size);
	Vector(const Vector<T> &vector);
	~Vector();
	Vector<T>& operator=(const Vector<T> &vector);
	T& operator[](size_t index) const;
	void push_back(const T& t);
	void insert(size_t index, const T& t);
	void erase(size_t index);
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
T& Vector<T>::operator[](size_t index) const {
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
void Vector<T>::erase(size_t index) {
	delete _data[index];
	for(size_t i=index;i<_elements-1;++i) {
		_data[i]=_data[i+1];
	}
	--_elements;
}

template<class T>
void Vector<T>::clear() {
	for(size_t i=0;i<_elements;++i) {
		delete _data[i];
	}
	_elements=0;
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
		_data[index]=new T(t);
		++_elements;
	} else {
		throw std::out_of_range("index");
	}
}

template<class T>
void Vector<T>::swap(T *a, T *b) {
	T* foo;
	foo = a;
	a = b;
	b = foo;
}

template<class T>
void Vector<T>::quicksort(T** data, int l, int r) {
	int pivot, i_left, i_right;
	i_left = l;
	i_right = r;
	
	if (l < r) {
		pivot = (l+r)/2;
		while((i_left <= pivot) && (i_right >= pivot)) {
			while((*data[i_left] < *data[pivot]) && (i_left <= pivot)) {
				i_left++;
			}
			while((*data[i_right] > *data[pivot]) && (i_right >= pivot)) {
				i_right--;
			}
			swap(data[i_left],data[i_right]);
			
			quicksort(data, l, i_right-1);
			quicksort(data, i_right, r);
		}
	}
}

template<class T>
void Vector<T>::sort(bool ascending) {
	// For now, only sort ascending.
	quicksort(_data, 0, _elements);
}

template<class T>
void Vector<T>::delete_data() {
	for(size_t i=0;i<_elements;++i) {
		delete _data[i];
	}
	delete[] _data;
}

#endif
