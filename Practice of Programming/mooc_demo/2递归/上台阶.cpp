#include<iostream>
#include<vector>
using namespace std;
vector<int>mem(100, -1);
int f(int n) {
	if (n == 0)
		return 1;
	if (n == 1)
		return 1;
	if (n == 2)
		return 2;
	if (mem[n] == -1)
		mem[n] = f(n - 1) + f(n - 2) + f(n - 3);
	return mem[n];
}
int main()
{
	int a;
	while (cin >> a) {
		if (a == 0)
			break;
		cout << f(a) << endl;
	}
	return 0;
}

