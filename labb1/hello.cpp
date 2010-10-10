#include <iostream>
#include <cstdlib>

using namespace std;
int main(int argc,char **argv) {
	cout<<"Hello";
	if(argc>1) {
		int num=atoi(argv[1]);
		char* word="world";
		if(argc>2)
			word=argv[2];
		else if(num==0)
			word=argv[1];
		if(num==0) {
			cout<<" "<<word;
		} else {
			for(int i=0;i<num;++i) {
				cout<<" "<<word;
			}
		}
	} else {
		cout<<" world";
	}
	cout<<"!"<<endl;
}
