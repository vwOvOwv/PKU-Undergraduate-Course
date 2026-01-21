//本题递推模拟即可，重点在于用位运算计算2^n
#include<iostream>
#include<cstring>
using namespace std;
char mem[1050][(1 << 11) + 10];//注意列数是2^11量级
void output(int n) {
	for (int i = 1029 - (1 << n) + 1; i <= 1029; i++) {
		for (int j = 1; j <= (1 << (n + 1)); j++) {
				cout << mem[i][j];
		}
		cout << endl;
	}
}
void f() {
	mem[1028][2] = '/';
	mem[1028][3] = '\\';
	mem[1029][1] = '/';
	mem[1029][4] = '\\';
	mem[1029][2] = '_';
	mem[1029][3] = '_';
	for (int k = 2; k <= 10; k++) {
		for (int i = 1029 - (1 << (k - 1)) + 1; i <= 1029; i++) {
			for (int j = 1; j <= (1 << k); j++) {
				if (mem[i][j] != ' ') {
					mem[i][j + (1 << k)] = mem[i][j];
					mem[i - (1 << (k - 1))][j + (1 << (k - 1))] = mem[i][j];
				}
			}
		}
	}
}
int main()
{
	int n;
	memset(mem, ' ', sizeof(mem));
	f();
	while (cin >> n) {
		if (n == 0)
			break;
		output(n);
		cout << endl;
	}
	return 0;
}
