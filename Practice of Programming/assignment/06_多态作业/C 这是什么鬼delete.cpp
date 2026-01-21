#include <iostream>
using namespace std;
class A
{
public:
	A() { }
	//
virtual ~A() { cout << "destructor A" << endl; }
	/*pa指向B, B由A派生而来故B中包含一个A类对象（显式指针指向A）
	delete pa时应调用A的析构函数，但A的析构函数是虚函数，因此执
	行pa指向的对象（B）的析构函数，而B包含一个指向A的指针，因此
	会再执行A的析构函数*/
	//
};
class B:public A {
	public:
	~B() { cout << "destructor B" << endl; }
};
int main()
{
	A * pa;
	pa = new B;
	delete pa;
	return 0;
}
