#include <iostream>
using namespace std;
class MyCin
{
	//
    bool judge;
public:
    MyCin():judge(true){}//初始化judge为true
    operator bool() { return judge; }//重载bool，使得其可以判断输入是否结束
    MyCin& operator>>(int &n) {//若用int n编译出错（实参无值）
        cin >> n;
        if (n == -1)//输入结束
            judge = false;
        return *this;
    }
    //
};
int main()
{
    MyCin m;
    int n1,n2;
    while( m >> n1 >> n2)
        cout  << n1 << " " << n2 << endl;
    return 0;
}
