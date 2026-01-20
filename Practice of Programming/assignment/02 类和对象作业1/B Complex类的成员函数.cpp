#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
class Complex {
private:
    double r,i;
public:
    void Print() {
        cout << r << "+" << i << "i" << endl;
    }
    //
 	Complex() {}
 	Complex(const char p[4]) {
        r = (double)p[0] - '0';
        i = (double)p[2] - '0';
    }
    //
};
int main() {
    Complex a;
    a = "3+4i"; a.Print();
    a = "5+6i"; a.Print();
    return 0;
}
