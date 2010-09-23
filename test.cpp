#include "Vector.h"
#include <iostream>
#include <stdexcept>

using namespace std;
int main() {
	Vector vector10(10);
	Vector vector100(100);
	Vector vector0(0);
    
	vector10[9] = 4711;
	vector100[42] = 573;
	vector100[0] = 1;
	vector100[99] = 5;
	
	cout << vector100[42] << endl;
	vector100[42] = vector10[9];
	cout << vector100[42] << endl;
	
	try {
		vector10[47] = 5;
	}
	catch (out_of_range& oor) {
		cerr << "Out of range error: " << oor.what() << endl;
	}
	
	Vector vectorCopy(0);
	vectorCopy = vector100;
	
	cout << vectorCopy[42] << endl;
	cout << vectorCopy[0] << endl;
	cout << vectorCopy[99] << endl;
	
	
}