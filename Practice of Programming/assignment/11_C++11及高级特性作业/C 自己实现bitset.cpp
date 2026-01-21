#include <iostream>
#include <cstring>
using namespace std;
template <int bitNum>
struct MyBitset
{
	char a[bitNum/8+1];
	MyBitset() { memset(a,0,sizeof(a));};
	void Set(int i,int v) {
		char & c = a[i/8];
		int bp = i % 8;
		if( v )
			c |= (1 << bp);
		else
			c &= ~(1 << bp);
	}
	//
	int Get(int i) {
		char  c = a[i / 8];
		int bp = i % 8;
		return (c >> bp) & 1;
	}
	class Bit {
	public:
		int i;
		MyBitset<bitNum>& bs;
		Bit(int k, MyBitset<bitNum>& b) :i(k), bs(b) { }
		Bit& operator = (int v) {
			bs.Set(i, v);
			return *this;
		}
		operator int() const {
			return bs.Get(i);
		}
		Bit& operator = (const Bit& r) {
			bs.Set(i, r);
			return *this;
		}
	};

	Bit operator [](int k) {
		return Bit(k, *this);
	}
	//
void Print() {
		for(int i = 0;i < bitNum; ++i)
			cout << (*this) [i];
		cout << endl;
	}

};

int main()
{
	int n;
	int i,j,k,v;
	while( cin >>  n) {
		MyBitset<20> bs;
		for(int i = 0;i < n; ++i) {
			int t;
			cin >> t;
			bs.Set(t,1);
		}
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		bs[i] = bs[j] = bs[k];
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		(bs[i] = bs[j]) = bs[k];
		bs.Print();
	}
	return 0;
}
