#include <iostream>
#include "setm.h"

using namespace std;

typedef mySet<11> Set;
int main(){
	Set a;
	a.set(1); a.set(3); a.set(5); a.set(7); a.set(9);
	Set b;
	b.set(2); b.set(4); b.set(6); b.set(8); b.set(10);
	Set c;
	c.set(1); c.set(2); c.set(3); c.set(4); c.set(5);
	Set d;
	d.set(5);
	Set e;
	cout << a << endl;
	cout << b << endl;
	cout << c << endl;
	cout << d << endl;
	cout << e << endl;
	//测试基本运算符
	cout << "a | b " << (a | b) << endl;
    cout << "a=" << a << " b=" << b << endl; 
	cout << "a & b " << (a & b) << endl;
	cout << "a - c " << (a - c) << endl;
	cout << "a ^ c " << (a ^ c) << endl;
	cout << "a == b " << (a == b) << endl;
	cout << "a != b " << (a != b) << endl;
	cout << "a == b " << (a == b) << endl;
	cout << "c <= a " << (c <= a) << endl;
	cout << "c < a  " << (c < a) << endl;
	cout << "a >= b " << (a >= b) << endl;
	cout << "a > d " << (a > d) << endl;
	cout << "~a  " << (~a) << endl;
	cout << "count() " << a.count() << endl;
	cout << "flip() " << e.flip() << endl;
	cout << "5 at a " << a.at(5) << endl;
	cout << "(a | b) - c " << ((a | b) - c) << endl;
	cout << "(a & c) = d " << ((a & c) == d) << endl;
	return 0;
}