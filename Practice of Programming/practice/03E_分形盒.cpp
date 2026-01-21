#include<iostream>
#include<cstring>
#include<algorithm>
#include<cmath>

using namespace std;
int n;
char ans[730][730];

void f(int n) {
	for (int x = 730 - pow(3, n - 1); x < 730; x++) {
		for (int y = 1; y <= pow(3, n - 1); y++) {
			if (ans[x][y] == 'X')
				cout << 'X';
			else
				cout << ' ';
		}
		cout << endl;
	}
	cout << '-' << endl;
}
int main()
{
	ans[729][1] = 'X';
	for (int i = 2; i <= 7; i++) {
		int w = pow(3, i - 2);
		for (int x = 729; x >= 730 - w; x--) {
			for (int y = 1; y <= w; y++) {
				if (ans[x][y] == 'X') {
					ans[x - 2 * w][y] = 'X';
					ans[x][y + 2 * w] = 'X';
					ans[x - w][y + w] = 'X';
					ans[x - 2 * w][y + 2 * w] = 'X';
				}
			}
		}
	}
	while (cin >> n) {
		if (n == -1)
			break;
		f(n);
	}
	return 0;
}
