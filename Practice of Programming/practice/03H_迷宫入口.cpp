#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>

using namespace std;
/*
本题重点在于确定搜索的对象
搜索每个正方形可以放在的位置or搜索每个位置可以放的正方形
显然后者情况更简洁，前者状态太多
*/
int t;
int s, n;
int cnt[11];//记录每个边长的正方形个数
bool lock[200][200];
bool flag;
bool isok(int i,int j,int k) {//判断能不能在(i,j)放一个边长为k的正方形
	if (cnt[k] == 0)
		return false;
	for (int r = i; r <= i + k - 1; r++) {
		for (int c = j; c <= j + k - 1; c++) {
			if (r<1 || r>s || c<1 || c>s)
				return false;
			if (lock[r][c])
				return false;
		}
	}
	return true;
}
void dfs() {
	if (flag)return;
	int i, j;
	for (i = 1; i <= s; i++) {
		for (j = 1; j <= s; j++) {
			if (!lock[i][j])
				goto loop;
		}
	}
	flag = true;
	return;
loop:;
	for (int k = 1; k <= 10; k++) {
		if (isok(i, j, k)) {
			cnt[k]--;
			for (int r = i; r <= i + k - 1; r++) {
				for (int c = j; c <= j + k - 1; c++) {
					if (c >= 1 && c <= s && r >= 1 && r <= s)
						lock[r][c] = true;
				}
			}
			dfs();
			for (int r = i; r <= i + k - 1; r++) {
				for (int c = j; c <= j + k - 1; c++) {
					if (c >= 1 && c <= s && r >= 1 && r <= s)
						lock[r][c] = false;
				}
			}
			cnt[k]++;
		}
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> t;
	while (t--) {
		flag = false;
		memset(cnt, 0, sizeof(cnt));
		memset(lock, 0, sizeof(lock));
		cin >> s >> n;
		for (int i = 1; i <= n; i++) {
			int l;
			cin >> l;
			cnt[l]++;
		}
		dfs();
		if (flag)
			cout << "YES" << endl;
		else
			cout << "NO" << endl;
	}
	return 0;
}
