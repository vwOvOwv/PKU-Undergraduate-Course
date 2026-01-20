#include<iostream>
#include<cstring>
#include<algorithm>

using namespace std;
int t, r, c;
char a[510][510];
int visited[510][510];
int dx[4] = { 0,1,0,-1 }, dy[4] = { 1,0,-1,0 };
void dfs(int i, int j) {
	for (int k = 0; k < 4; k++) {
		int x = i + dx[k], y = j + dy[k];
		if (x >= 1 && x <= r && y >= 1 && y <= c && a[x][y]=='O'&&visited[x][y] == 0) {
			visited[x][y] = 1;
			dfs(x, y);
		}
	}
}
int main()
{
	cin >> t;
	while (t--) {
		cin >> r >> c;
		memset(a, 0, sizeof(a));
		memset(visited, 0, sizeof(visited));
		for (int i = 1; i <= r; i++) {
			for (int j = 1; j <= c; j++) {
				cin >> a[i][j];
			}
		}
		for (int i = 1; i <= r; i++) {
			if (a[i][1] == 'O') {
				visited[i][1] = 1;
				dfs(i, 1);
			}
			if (a[i][c] == 'O') {
				visited[i][c] = 1;
				dfs(i, c);
			}
		}
		for (int j = 1; j <= c; j++) {
			if (a[1][j] == 'O') {
				visited[1][j] = 1;
				dfs(1, j);
			}
			if (a[r][j] == 'O') {
				visited[r][j] = 1;
				dfs(r, j);
			}
		}
		for (int i = 1; i <= r; i++) {
			for (int j = 1; j <= c; j++) {
				if (visited[i][j] == 1)
					cout << 'O';
				else
					cout << 'X';
			}
			cout << endl;
		}
		cout << endl;
	}
	return 0;
}
