#include <iostream>
using namespace std;

bool Greater2(int n1,int n2)
{
	return n1 > n2;
}
bool Greater1(int n1,int n2)
{
	return n1 < n2;
}
bool Greater3(double d1,double d2)
{
	return d1 < d2;
}

template <class T1,class T2>
void mysort(
//
T1 *p_start,T1 *p_end,T2 func) {
    for (T1* i = p_start; i < p_end; i++) {
        for (T1* j = p_start; j < p_end; j++) {//这是最傻的排序方法、、、
            if (func(*i,*j)) {//注意i、j顺序
                T1 tmp = *i;
                *i = *j;
                *j = tmp;
            }
        }
    }
}
//
#define NUM 5
int main()
{
    int an[NUM] = { 8,123,11,10,4 };
    mysort(an,an+NUM,Greater1); //从小到大排序
    for( int i = 0;i < NUM; i ++ )
       cout << an[i] << ",";
    mysort(an,an+NUM,Greater2); //从大到小排序
    cout << endl;
    for( int i = 0;i < NUM; i ++ )
        cout << an[i] << ",";
    cout << endl;
    double d[6] = { 1.4,1.8,3.2,1.2,3.1,2.1};
    mysort(d+1,d+5,Greater3); //将数组从下标1到下标4从小到大排序
    for( int i = 0;i < 6; i ++ )
         cout << d[i] << ",";
	return 0;
}
