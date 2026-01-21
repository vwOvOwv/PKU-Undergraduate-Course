#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<climits>
#include<vector>
using namespace std;

int n, k;
int a[110];
int f[1010];

int main()
{
	while (cin >> n >> k) {
		if (n == 0 && k == 0)
			break;
		memset(a, 0, sizeof(a));
		memset(f, 0, sizeof(f));
		for (int i = 0; i < n; i++)
			cin >> a[i];
		f[0] = 0;
		for (int i = 1; i <= k; i++) {
			f[i] = INT_MAX-10;//注意避免int溢出
		}
		for (int i = 1; i <= n; i++) {
			for (int j = a[i]; j <= k; j++) {
				f[j] = min(f[j - a[i]] + 1, f[j]);//完全背包问题的变种
			}
		}
		if (f[k] != INT_MAX-10)
			cout << f[k] << endl;
		else
			cout << "-1" << endl;
	}
	return 0;
}

