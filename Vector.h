#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <cstring>
#define DEFAULT_SIZE 100

template <class T>
class Vector {
	T** _data;
	size_t _size;
	size_t _elements;

	void resize();

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

#endif
