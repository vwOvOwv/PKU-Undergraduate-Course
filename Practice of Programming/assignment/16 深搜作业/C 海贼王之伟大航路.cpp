#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
int n;
int t[20][20];
int mini = 1 << 30;
int ans = 0;
int visited[20];//记录是否已经走过
int record[20][1 << 18];//本题重点！record[i][j]表示从状态j走到i的最短路径，要借助这个进行剪枝，否则tle
int state[20];//规定每个岛屿的状态，这里用到了整数的二进制分解
int s;//计算状态
void dfs(int i) {
	bool flag = false;//标记还有没有没走到的
	for (int j = 1; j <= n - 1; j++) {//注意是n-1，最后走到n
		if (j == i)
			continue;
		if (visited[j] == 0) {
			flag = true;//有没走到的
			if (record[j][s+state[j]]==0||(ans + t[i][j] < record[j][s + state[j]])) {
				record[j][s+state[j]] = ans + t[i][j];//更新状态
				s += state[j];
				visited[j] = 1;
				ans += t[i][j];
				dfs(j);
				ans -= t[i][j];
				visited[j] = 0;
				s -= state[j];
			}
		}
	}
	if (!flag) {//都走完了，dfs终点
		mini = min(mini, ans + t[i][n]);
		return;
	}
}
int main()
{
	cin >> n;
	int tmp = 2;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> t[i][j];
		}
	}
	for (int i = 1; i <= n; i++) {//用2的正整数倍标记状态，从而对每一个岛屿的组合状态值都不同
		state[i] = tmp;
		tmp *= 2;
	}
	visited[1] = 1;
	s = state[1];//注意初始化状态
	dfs(1);
	cout << mini;
}
