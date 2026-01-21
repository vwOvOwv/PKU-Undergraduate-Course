#include <iostream>
#include <string>
using namespace std;
template <class T>
T SumArray(//
T* start,T *end){
	T tmp = *start;
	for (T *i = start+1; i <= end-1; ++i) {
		tmp += *i;
	}
	return tmp;
	//
}
int main() {
	string array[4] = { "Tom","Jack","Mary","John"};
	cout << SumArray(array,array+4) << endl;
	int a[4] = { 1, 2, 3, 4};  //ÌáÊ¾£º1+2+3+4 = 10
	cout << SumArray(a,a+4) << endl;
	return 0;
}
