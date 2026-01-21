#include <iostream>
using namespace std;
// 在此处补充你的代码
template<class T1,class T2>
class g {
public:
	T1 a;
	g(T1 x):a(x){}
	T2 operator()(T2 y) {
		T2 ans = y + a;
		return ans;
	}
};
template<class T1,class T2>
g<T1,T2> f(T1 x) {
	return g<T1, T2>(x);
}
//

int main()
{
   cout << f<int,int>(7)(9) << endl;   //16
   cout << f<string,string> (" hello!")("world")  <<endl; // world hello!
   cout << f<char,string> ('!')("world") << endl;


   return 0;    //world!
}
