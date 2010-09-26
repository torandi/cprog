#include "vector.h"
#include <iostream>
#include <stdexcept>

using namespace std;
int main() {
	Vector<int> v;
	v.push_back(123);
	cout<<v[0]<<endl;
	v.push_back(3);
	cout<<v[0]<<", "<<v[1]<<endl;
	v.insert(0,5);
	cout<<v[0]<<", "<<v[1]<<", "<<v[2]<<endl;
	for(int i=0;i<100000;++i) {
		v.push_back(i);
	}
	v.insert(39201,42);
	cout<<v[13920]<<endl;
	v.sort();
}
