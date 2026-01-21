#include <iostream>
#include <list>
#include <string>
using namespace std;

template <class T1,class T2>
void Copy(T1 s,T1 e, T2 x)
{
	for(; s != e; ++s,++x)
		*x = *s;
}


template<class T>
class myostream_iteraotr
{
	//
private:
	string sep;
	ostream& o;
	T array[5];//直接用题中的提示，怎么简单怎么来
	int i;
public:
	myostream_iteraotr(ostream& s, string a) :sep(a), o(s){
		i = 0;
	}
	T& operator*() {
		return array[i];
	}
	T* operator++() {
		i++;
		if (i == 5)//已经全部存入了，打印出来
		{
			for (int j = 0; j < 5; j++)
				o << array[j] << sep;
		}
		return &array[i];
	}
	//
};


int main()
{	const int SIZE = 5;
	int a[SIZE] = {5,21,14,2,3};
	double b[SIZE] = { 1.4, 5.56,3.2,98.3,3.3};
	list<int> lst(a,a+SIZE);
	myostream_iteraotr<int> output(cout,",");
	Copy( lst.begin(),lst.end(),output);
	cout << endl;
	myostream_iteraotr<double> output2(cout,"--");
	Copy(b,b+SIZE,output2);
	return 0;
}
