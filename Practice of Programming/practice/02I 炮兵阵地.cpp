//经典的状压dp问题，与二进制枚举类似
//预处理是关键
//本题数据较弱，不优化可以AC
//可以用滚动数组优化：dp[110][1 << 10][1 << 10]->dp[2][1<<10][1<<10],dp[0]记录前一层，dp[1]记录本层
#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>
#include<queue>

using namespace std;

int n, m;
int dp[2][1 << 10][1 << 10];//dp[i][j][k]表示第i行状态为j第i-1行状态为k时最多摆放的炮兵数
int map[110];//二进制地图
vector<int>pre[1 << 10];//预处理每行可行的摆放状态
vector<int>state;//记录所有可能的状态(只要本行可行即算作可能的状态)
int cnt[1 << 10];//记录每种状态的炮兵数
string s;//输入的字符串
int count(int a) {
	int ans = 0;
	for (int i = 0; i < 10; i++) {
		if (a & (1 << i))
			ans++;
	}
	return ans;
}
bool check(int a) {//检查是否在本行冲突
	for (int i = 0; i < m; i++)
		if (a & (a << 1) || a & (a << 2))
			return false;
	return true;
}
int main()
{
	int ans = 0;
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> s;
		for (int j = 0; j < m; j++) {
			if (s[j] == 'H') {
				map[i] |= (1 << j);
			}
		}
	}
	//记录所有可能状态
	for (int i = 0; i < 1 << m; i++) {
		if (check(i)) {
			state.push_back(i);
			cnt[i] = count(i);
		}
	}
	//预处理能转移到状态i的所有状态
	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < state.size(); j++) {
			if (!(state[i] & state[j]))
				pre[state[i]].push_back(state[j]);
		}
	}
	for (int i = 1; i <= n + 2; i++) {//枚举每一层，注意由于第i层与第i-1,i-2层相关，故i要到n+2
		for (auto i_state : state) {//枚举第i层，用给予范围的for循环可以大大简化写法
			if(!(i_state&map[i]))//合法
				for(auto j_state:pre[i_state])
					if(!(j_state&map[i-1]))
						for(auto k_state:pre[j_state])
							if ((i >= 2 && !(k_state & map[i - 2])) || (i < 2)) {
								if (!(i_state & k_state))
									dp[i&1][i_state][j_state] = max(dp[i&1][i_state][j_state], dp[i - 1&1][j_state][k_state] + cnt[i_state]);//取一个最大的
							}
		}
	}
	cout << dp[n + 2&1][0][0] << endl;//注意因为map[i+1]=map[i+2]=0,所以除了一个炮兵都不放(0)没有合法状态了
	return 0;
}
