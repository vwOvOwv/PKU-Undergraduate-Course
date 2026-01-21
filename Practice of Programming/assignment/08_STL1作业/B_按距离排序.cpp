#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
using namespace std;
template <class T1,class T2>
struct Closer {
	//
private:
	T1 x;//参照值
	T2 op;//比较器（函数指针）
public:
	Closer(T1 y, T2 p) :x(y), op(p) {}
	bool operator()(const T1& x1, const T1& x2) {//比较的规则
		int dis1 = op(x1, x);
		int dis2 = op(x2, x);
		if (dis1 < dis2)
			return true;
		else if (dis1 > dis2)
			return false;
		else {
			if (x1 < x2)
				return true;
			else
				return false;
		}
	}
	//
};

int Distance1(int n1,int n2) {
	return abs(n1-n2);
}
int Distance2(const string & s1, const string & s2)
{
	return abs((int)s1.length()- (int) s2.length());
}
int a[10] = { 0,3,1,4,7,9,20,8,10,15};
string b[6] = {"American","Jack","To","Peking","abcdefghijklmnop","123456789"};
int main()
{
	int n;string s;
	while( cin >> n >> s ) {
		sort(a,a+10,Closer<int ,int (*)(int ,int)> (n,Distance1));
		for(int i = 0;i < 10; ++i)
			cout << a[i] << "," ;
		cout << endl;
		sort(b,b+6,Closer<string,int (*)(const string &,const string &  )> (s,Distance2));
		for(int i = 0;i < 6; ++i)
			cout << b[i] << "," ;
		cout << endl;
	}
	return 0;
}
