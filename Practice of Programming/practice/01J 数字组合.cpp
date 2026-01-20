#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
int a[30];
int f[30][1010];//f[i][j]表示从前i个数中选出j
int main()
{
	int n, t;
	cin >> n >> t;
	for (int i = 1; i <= n; i++)
		cin >> a[i];
	//sort(a + 1, a + n + 1);
	for (int i = 0; i <= n; i++)
		f[i][0] = 1;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= t; j++) {//类似于背包问题求方案数
			f[i][j] = f[i - 1][j];//不选a[i]
			if (j >= a[i])
				f[i][j] += f[i - 1][j - a[i]];//选a[i]
		}
	}
	cout << f[n][t];
	return 0;
}
