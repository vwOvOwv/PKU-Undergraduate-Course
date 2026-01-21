#include <iostream>
using namespace std;
class A {
public:
	int val;

	A(int//
 i) {
		val = i;
	}
	A& GetObj() {
		this->val = val;
		return*this;
	}
A() {
		val = 123;
	};
	//
};
int main()
{
	int m,n;
	A a;
	cout << a.val << endl;
	while(cin >> m >> n) {
		a.GetObj() = m;
		cout << a.val << endl;
		a.GetObj() = A(n);
		cout << a.val<< endl;
	}
	return 0;
}
