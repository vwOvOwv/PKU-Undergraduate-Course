#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>

using namespace std;

int m, n, k;
int a[30][30];
vector<vector<int>> visited(30, vector<int>(30));
int cnt = 1;
int maxi;
int dx[4] = { 0,1,0,-1 }, dy[4] = { 1,0,-1,0 };
void dfs(int i, int j, vector<vector<int>>visited) {
	visited[i][j] = 1;
	for (int r = 1; r <= m; r++) {
		for (int c = 1; c <= n; c++) {
			if (a[r][c] == a[i][j])
				visited[r][c] = 1;
		}
	}
	for (int k = 0; k < 4; k++) {
		int x = i + dx[k], y = j + dy[k];
		if (x >= 1 && x <= m && y >= 1 && y <= n && visited[x][y] == 0) {
			cnt++;
			dfs(x, y, visited);
			cnt--;
		}
	}
	maxi = max(maxi, cnt);
}
int main()
{
	cin >> m >> n >> k;
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> a[i][j];
		}
	}
	maxi = 1;
	dfs(1, 1, visited);
	cout << maxi;
	return 0;
}
