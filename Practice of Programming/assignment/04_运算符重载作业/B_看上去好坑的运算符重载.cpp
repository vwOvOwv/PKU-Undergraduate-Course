#include <iostream>
using namespace std;
class MyInt
{
	int nVal;
	public:
	MyInt( int n) { nVal = n ;}
	//
	MyInt& operator-(int n) {
		nVal -= n;
		return *this;
	}
	friend int Inc(MyInt o) {
		return o.nVal + 1;
	}
	//
};
int Inc(int n) {
	return n + 1;
}
int main () {
	int n;
	while(cin >>n) {
		MyInt objInt(n);
		objInt-2-1-3;
		cout << Inc(objInt);
		cout <<",";
		objInt-2-1;
		cout << Inc(objInt) << endl;
	}
	return 0;
}
