#include <iostream>
#include <string>

using namespace std;
template <class T>
class CMyistream_iterator
{
	//
private:
	T* p;
public:
	CMyistream_iterator(istream& c) {
		p = new T;//动态分配一个T的内存，不要写为
		/* T n;
		* cin>>n;
		* p=&n;
		这样n是临时对象，消亡后p就不知道指向哪里了
		*/
		cin >> *p;
	}
    T operator*() {
		return *p;
	}
	CMyistream_iterator operator++(int k) {//后置++
		cin >> *p;
		return *this;
	}
//迭代器
//
};



int main()
{
	int t;
	cin >> t;
	while( t -- ) {
		 CMyistream_iterator<int> inputInt(cin);
		 int n1,n2,n3;
		 n1 = * inputInt; //读入 n1
		 int tmp = * inputInt;
		 cout << tmp << endl;
		 inputInt ++;
		 n2 = * inputInt; //读入 n2
		 inputInt ++;
		 n3 = * inputInt; //读入 n3
		 cout << n1 << " " << n2<< " " << n3 << " ";
		 CMyistream_iterator<string> inputStr(cin);
		 string s1,s2;
		 s1 = * inputStr;
		 inputStr ++;
		 s2 = * inputStr;
		 cout << s1 << " " << s2 << endl;
	}
	 return 0;
}
