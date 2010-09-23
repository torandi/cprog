#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <cstring>

template <class T>
class Vector {
	T** _data;
	size_t _size;

public:
	explicit Vector(size_t size);
	Vector(const Vector<T> &vector);
	~Vector();
	Vector<T>& operator=(const Vector<T> &vector);
	T& operator[](unsigned int index) const;
};

#endif