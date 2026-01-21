#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
//状压dp
const int MAX = 1 << 16;//用二进制表示所有的状态
int t, n;
string s[20];
int d[20], c[20];
int dp[MAX];//每个状态下的最优解，即最少扣分数
string ans[MAX];//每个状态的路径记录
int sum[MAX];//达到某个状态所需的时间
int main()
{
	cin >> t;
	while (t--) {
		cin >> n;
		for (int i = 0; i < n; i++) {
			cin >> s[i] >> d[i] >> c[i];
			sum[1 << i] = c[i];//只完成 i，总时间即为c[i]
		}
		for (int i = 1; i < (1 << n); i++) {//枚举所有状态，注意i的取值范围
			for (int j = 0; j < n; j++) {
				if (i & (1 << j))//已经完成了j
					sum[i] = sum[i ^ (1 << j)] + c[j];//从没完成j的状态转移过来
			}
		}
		dp[0] = 0;
		for (int i = 0; i < (1 << n); i++)
			ans[i] = "";
		for (int i = 1; i < (1 << n); i++) {
			for (int j = 0; j < n; j++) {
				int tmp = sum[i] - d[j];
				if (tmp < 0)
					tmp = 0;
				if (i & (1 << j) && (dp[i ^ (1 << j)] + tmp <= dp[i] || ans[i] == "")) {
					if (dp[i ^ (1 << j)] + tmp < dp[i] || ans[i ^ (1 << j)] + s[j] + "\n" < ans[i]||ans[i]=="") {
						ans[i] = ans[i ^ (1 << j)] + s[j] + "\n";
					}
					dp[i] = dp[i ^ (1 << j)] + tmp;
				}
			}
		}
		cout << dp[(1 << n) - 1] << endl;
		cout << ans[(1 << n) - 1];
	}
	return 0;
}
