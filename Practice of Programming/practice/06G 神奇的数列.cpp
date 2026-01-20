#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>

using namespace std;
int t, n, a[210];
int f[210][210];//dp[i][j]表示从a[i]到a[j]能分割出的最少数据段的个数
/*
区间dp问题，外层枚举区间长度，内层枚举起点，注意起点与长度的关系从而得到终点
本题中应当在起点i与终点j之间找a[k]=a[j]的点，然后把中间的部分消掉
剩下的a[j]与前面的a[i]->a[k]合并起来一起消除，数据段数仍为dp[i][k]
for(int len=2;len<=n;len++){
	for(int i=1;i+len-1<=n;i++){//起点
		int j=i+len-1;//终点
		for(int k=i;k<=j;k++){
			if(a[k]==a[j])
				f[i][j]=min(f[i][j],f[i][k]+f[k+1][j-1]);
			else
				f[i][j]=min(f[i][j],f[i][k]+f[k+1][j]);
		}
	}
}
*/

int main() {
	cin >> t;
	for (int x = 1; x <= t; x++) {
		cin >> n;
		for (int i = 1; i <= n; i++)
			cin >> a[i];
		for (int i = 1; i <= n; i++) {
			for (int j = i; j <= n; j++) {
				f[i][j] = 1 << 30;
			}
		}
		for (int i = 1; i <= n; i++)
			f[i][i] = 1;
		for (int len = 2; len <= n; len++) {
			for (int i = 1; i + len - 1 <= n; i++) {
				int j = i + len - 1;
				for (int k = i; k <= j; k++) {
					if (a[k] == a[j])
						f[i][j] = min(f[i][j], f[i][k] + f[k + 1][j - 1]);
					else
						f[i][j] = min(f[i][j], f[i][k] + f[k + 1][j]);
				}
			}
		}
		cout << "Case " << x << ": " << f[1][n] << endl;
	}
	return 0;
}

