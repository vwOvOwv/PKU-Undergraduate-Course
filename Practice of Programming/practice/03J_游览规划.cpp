#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>

using namespace std;

int m, n, s;
int fun[110], cost[110], t[110], is_limited[110];
int f[1010][1010];//二重费用背包
int main()
{
	cin >> m >> n >> s;
	for (int i = 1; i <= n; i++) {
		string tmp;
		cin >> fun[i] >> cost[i] >> t[i] >> tmp;
		if (tmp[0] == 'l')
			is_limited[i] = 1;
	}
	int ans = 0;
	for (int i1 = 0; i1 <= s / 100; i1++) {
		for (int i2 = 0; i2 <= s / 150; i2++) {
			for (int i3 = 0; i3 <= s / 200; i3++) {//枚举所有方案
				if (s >= 100 * i1 + 150 * i2 + 200 * i3) {
					int sum_time = 24 * i1 + 48 * i2 + 72 * i3;
					int sur_money = s - 100 * i1 - 150 * i2 - 200 * i3;
					memset(f, 0, sizeof(f));
					for (int i = 1; i <= n; i++) {
						if (is_limited[i]) {//0-1背包
							for (int j = sum_time; j >= t[i]; j--) {
								for (int k = sur_money; k >= cost[i]; k--) {
									f[j][k] = max(f[j][k], f[j - t[i]][k - cost[i]] + fun[i]);
								}
							}
						}
						else {//完全背包
							for (int j = 0; j <= sum_time; j++) {
								for (int k = 0; k <= sur_money; k++) {
									if (j >= t[i] && k >= cost[i])
										f[j][k] = max(f[j][k], f[j - t[i]][k - cost[i]] + fun[i]);
								}
							}
						}
					}
					ans = max(ans, f[sum_time][sur_money]);
				}
			}
		}
	}
	cout << ans;
	return 0;
}
