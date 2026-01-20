#include<iostream>
#include<cmath>
#include<iomanip>
#include<climits>
#include<cstring>
#include<algorithm>
#include<vector>
using namespace std;
int maze[5][5];
int flag[5][5];//用来记录是否走过
vector<pair<int, int> > ans;
vector<pair<int, int>>rec;
void dfs(int i, int j, vector<pair<int, int>>rec) {
	if (i == 4 && j == 4) {//抵达终点
		if (ans.empty()) {
			for (int i = 0; i < rec.size(); i++)
				ans.push_back(rec[i]);
		}
		else {
			if (ans.size() > rec.size()) {
				ans.clear();
				for (int i = 0; i < rec.size(); i++)
					ans.push_back(rec[i]);
			}
		}
		return;
	}
	if (flag[i][j] == 1)//已经走过
		return;
	flag[i][j] = 1;//更新状态
	rec.push_back(make_pair(i, j));//记录路径
	if (i - 1 >= 0 && maze[i - 1][j] == 0&&flag[i-1][j]==0) {
		dfs(i - 1, j, rec);
		flag[i - 1][j] = 0;//注意回溯
	}
	if (i + 1 < 5 && maze[i + 1][j] == 0 && flag[i+1][j] == 0) {
		dfs(i + 1, j, rec);
		flag[i + 1][j] = 0;
	}
	if (j - 1 >= 0 && maze[i][j - 1] == 0 && flag[i][j-1] == 0) {
		dfs(i, j - 1, rec);
		flag[i][j - 1] = 0;
	}
	if (j + 1 < 5 && maze[i][j + 1] == 0 && flag[i][j+1] == 0) {
		dfs(i, j + 1, rec);
		flag[i][j + 1] = 0;
	}
	return;
}
int main()
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int s;
			cin >> maze[i][j];
		}
	}
	dfs(0, 0, rec);
	for (int i = 0; i < ans.size(); i++)
		cout << '(' << ans[i].first << ", " << ans[i].second << ')' << endl;
	cout << "(4, 4)" << endl;
	return 0;
}
