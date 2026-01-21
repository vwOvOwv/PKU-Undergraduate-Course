#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
int n, x;
int a[210];
int dp[10010];//dp[i]表示钱数等于i时的所有方案数
int rec[10010];//rec[i]记录钱数为i时不选某个硬币的方案数！！
int cnt = 0;
int ans[10010];//记录必须选的硬币的编号
int main()
{
	cin >> n >> x;
	for (int i = 1; i <= n; i++)
		cin >> a[i];
	dp[0] = 1;//重要
	for (int i = 1; i <= n; i++) {//与背包问题形式类似，但是要算的是总方案数
		for (int j = x; j >= a[i]; j--)
			dp[j] += dp[j - a[i]];//对每一个j>=a[i]，都是选了a[i]凑成了j
	}
	for (int i = 1; i <= n; i++) {
		memset(rec, 0, sizeof(rec));
		for (int j = 0; j <= x; j++) {
			if (j < a[i])
				rec[j] = dp[j];//一定没选a[i]
			else//可以选a[i]
				rec[j] = dp[j] - rec[j - a[i]];//总方案数-选了a[i]到达j的方案数，注意观察dp数组的递推模式
		}
		if (rec[x] == 0) {//不选a[i]不行
			ans[cnt] = a[i];
			cnt++;
		}
	}
	cout << cnt << endl;
	if (cnt != 0) {
		for (int i = 0; i < cnt; i++)
			cout << ans[i] << ' ';
	}
	cout << endl;
	return 0;
}
