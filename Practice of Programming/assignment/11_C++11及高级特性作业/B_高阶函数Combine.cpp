#include <iostream>
using namespace std;
// 在此处补充你的代码
template<class T1,class T2,class T3>
class combine {
    T1 a;
    T2 b;
public:
    combine(T1 x,T2 y):a(x),b(y){}
    double operator()(double num) {
        return a(a(num) + b(num));
    }
};
//

int main()
{
    auto Square = [] (double a) { return a * a; };
    auto Inc = [] (double a) { return a + 1; };
    cout << combine<decltype(Square),decltype(Inc),int>(Square,Inc)(3) << endl;
    cout << combine<decltype(Inc),decltype(Square),double>(Inc,Square)(2.5) << endl;

    return 0;
}
