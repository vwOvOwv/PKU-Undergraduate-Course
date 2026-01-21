#include<iostream>
#include<algorithm>
#include<climits>
#include<cstring>
using namespace std;
int h[110][110] = { 0 };
int mem[110][110] = { 0 };
int maxlen = 0;
int f(int i, int j) {
	if (mem[i][j] != -1)
		return mem[i][j];
	if (h[i][j] < h[i + 1][j] && h[i][j] < h[i - 1][j] && h[i][j] < h[i][j - 1] && h[i][j] < h[i][j + 1]) {
		mem[i][j] = 1;
		return mem[i][j];
	}
	int maxi = 0;
	if (h[i][j] > h[i + 1][j]) {
		int tmp = f(i + 1, j);
		maxi = max(maxi, tmp + 1);//注意应先+1
	}
	if (h[i][j] > h[i - 1][j]) {
		int tmp = f(i - 1, j);
		maxi = max(maxi, tmp + 1);
	}
	if (h[i][j] > h[i][j - 1]) {
		int tmp = f(i, j - 1);
		maxi = max(maxi, tmp + 1);
	}
	if (h[i][j] > h[i][j + 1]) {
		int tmp = f(i, j + 1);
		maxi = max(maxi, tmp + 1);
	}
	mem[i][j] = maxi;//不能在这里+1！
	return mem[i][j];
}
int main()
{
	int r, c;
	cin >> r >> c;
	memset(h, INT_MAX, sizeof(h));
	memset(mem, -1, sizeof(mem));
	for (int i = 1; i <= r; i++)
		for (int j = 1; j <= c; j++) {
			cin >> h[i][j];
		}
	for(int i=1;i<=r;i++)
		for (int j = 1; j <= c; j++) {
			maxlen = max(maxlen, f(i, j));
		}
	cout << maxlen;
	return 0;
}
