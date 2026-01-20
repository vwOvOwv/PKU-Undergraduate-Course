#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>

using namespace std;

int t, n;
long long int c;
long long dp[30][30][2];
//dp[i][j][0]表示i长度，第j大的数作为开头的"高低高低高"型的方案数
//dp[i][j][1]表示i长度，第j大的数作为开头的"低高低高低"型的方案数
//dp[i+1][j][1]=sum(dp[i][k][0]),j<=k<=i
//dp[i+1][j][0]=sum(dp[i][k][1]),1<=k<j
//关键在于如何从方案数还原出所需要的方案
bool flag[30];//记录是否用过
int res[30];

int main()
{
	dp[1][1][1] = dp[1][1][0] = 1;
	for (int i = 2; i <= 20; i++) {
		for (int j = 1; j <= i; j++) {
			for (int k = j; k <= i; k++) {
				dp[i][j][1] += dp[i - 1][k][0];//注意k可以取到j，否则递推有问题(怎么理解？)
			}
			for (int k = 1; k < j; k++) {
				dp[i][j][0] += dp[i - 1][k][1];
			}
		}
	}
	cin >> t;
	while (t--) {
		cin >> n >> c;
		memset(flag, 0, sizeof(flag));
		memset(res, 0, sizeof(res));
		//还原出对应方案
		//枚举当前未使用过的木棍
		/*
		假设第1短的木棒作为第一根，看此时的方案数P(1)是否>=C,如果否，则应该用第二短的作为第一根,
		C 减去P(1)，再看此时方案数P(2)和C比如何。如果还< C ，则应以第三短的作为第一根,C再减去P(2) ….
		若发现 第 i短的作为第一根时，方案数已经不小于C，则确定应该以第i短的作为第一根，
		C减去第 i短的作为第一根的所有方案数，然后再去确定第二根….
		*/
		long long int skipped = 0;
		for (int i = 1; i <= n; i++) {//枚举所有位置
			long long int old_val = skipped;
			int no = 0;
			for (int j = 1; j <= n; j++) {//枚举i位置的高度
				old_val = skipped;
				if (!flag[j]) {
					no++;
					if (i == 1) //特判
						skipped += (dp[n][no][0] + dp[n][no][1]);
					else {
						if (j > res[i - 1] && (i <= 2 || res[i - 2] > res[i - 1]))//高低 高
							skipped += dp[n - i + 1][no][0];//从i位置起是高低高
						if (j < res[i - 1] && (i <= 2 || res[i - 2] < res[i - 1]))//低高 低
							skipped += dp[n - i + 1][no][1];//从i位置起是低高低
					}
					if (skipped >= c) {//当前的j中包含着第c种排列
						res[i] = j;
						flag[j] = true;
						skipped = old_val;//注意更新，从而可以继续向后面的位置查找
						break;
					}
				}
			}
		}
		for (int i = 1; i <= n; i++)
			cout << res[i] << ' ';
		cout << endl;
	}
	return 0;
}

