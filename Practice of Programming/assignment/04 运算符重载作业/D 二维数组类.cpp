#include <iostream>
#include <cstring>
using namespace std;

class Array2 {
	//
private:
    int tmp = 0;
    int n, m;
    int* pr;
    int flag = 0;
public:
    Array2() {};
    Array2(int i, int j) {
        n = i;
        m = j;
        pr = new int[i * j ];
    }
    int* operator[](int k) {
        return &pr[k * m];
    }
    int operator()(int i, int j) {
        return pr[i * m + j];
    }
    Array2& operator=(const Array2& a) {
        n = a.n;
        m = a.m;
        pr = new int[n * m];
        for(int i=0;i<n;++i)
            for (int j = 0; j < m; ++j) {
                pr[i * m + j] = a.pr[i * m + j];
            }
        return *this;
    }
    //
};

int main() {
    Array2 a(3,4);
    int i,j;
    for(  i = 0;i < 3; ++i )
        for(  j = 0; j < 4; j ++ )
            a[i][j] = i * 4 + j;
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << a(i,j) << ",";
        }
        cout << endl;
    }
    cout << "next" << endl;
    Array2 b;     b = a;
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << b[i][j] << ",";
        }
        cout << endl;
    }
    return 0;
}
