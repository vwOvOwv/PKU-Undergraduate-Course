#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;
const int MAX = 110;
class CHugeInt {
	//
private:
	int num[210] = { 0 };
	int size = 0;
public:
	static void calculate(int num1[210], int num2[210], int &size1, int &size2) {
		if (size1 < size2)
			size1 = size2;
		for (int i = 0; i <size1; i++) {
			num1[i] += num2[i];
			if (num1[i] >= 10) {
				num1[i + 1]++;
				num1[i] -= 10;
				if (i + 1 >= size1)
					size1++;
			}
		}
	}
	CHugeInt(char s[210]) {
		size = strlen(s);
		for (int i = size - 1, j = 0; i >= 0; i--, j++) {
			num[j] = s[i] - '0';
		}
	}
	CHugeInt(int n) {
		int i = 0;
		memset(num, 0, sizeof(num));
		size = 0;
		while (n != 0) {
			num[i] = n % 10;
			size++;
			i++;
			n /= 10;
		}
	}
	CHugeInt(const CHugeInt& a) {
		memset(num, 0, sizeof(num));
		size = a.size;
		int i;
		for (i = 0; i < a.size; i++) {
			num[i] = a.num[i];
		}
	}
	~CHugeInt() {}
	friend ostream& operator<<(ostream& o, CHugeInt x) {
		for (int i = x.size - 1; i >= 0; i--)
			o << x.num[i];
			return o;
	}
	CHugeInt operator+(CHugeInt b) {
		CHugeInt tmp(*this);
		calculate(tmp.num, b.num, tmp.size, b.size);
		return tmp;
	}
	friend CHugeInt operator+(int n, CHugeInt& a) {
		CHugeInt tmp(n);
		calculate(tmp.num, a.num, tmp.size, a.size);
		return tmp;
	}
	CHugeInt& operator+=(int n) {
		CHugeInt tmp(n);
		calculate(num, tmp.num, size, tmp.size);
		return *this;
	}
	CHugeInt operator++() {
		CHugeInt tmp(1);
		calculate(num, tmp.num, size, tmp.size);
		return *this;
	}
	CHugeInt operator++(int k) {
		CHugeInt tmp(1);
		CHugeInt temp(*this);
		calculate(num, tmp.num, size, tmp.size);
		return temp;
	}
	//
};
int  main()
{
	char s[210];
	int n;

	while (cin >> s >> n) {
		CHugeInt a(s);
		CHugeInt b(n);

		cout << a + b << endl;
		cout << n + a << endl;
		cout << a + n << endl;
		b += n;
		cout  << ++ b << endl;
		cout << b++ << endl;
		cout << b << endl;
	}
	return 0;
}
