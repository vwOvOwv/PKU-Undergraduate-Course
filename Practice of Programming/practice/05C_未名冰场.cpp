#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>

using namespace std;

int n, m;
char a[110][110];
int visited[110][110];
int cnt;
struct Node
{
	int x, y;
	Node() {}
	Node(int xx, int yy) :x(xx), y(yy) {}
};
void init() {
	memset(a, 0, sizeof(a));
	memset(visited, 0, sizeof(visited));
	cnt = 0;
}
int dx[8] = { 0,1,0,-1,1,-1,1,-1 }, dy[8] = { 1,0,-1,0,1,-1,-1,1 };
void bfs(int i, int j) {
	queue<Node>q;
	q.push(Node(i, j));
	while (!q.empty()) {
		Node p = q.front();
		for (int k = 0; k < 8; k++) {
			int x = p.x + dx[k];
			int y = p.y + dy[k];
			if (x >= 1 && x <= n && y >= 1 && y <= m && !visited[x][y]) {
				visited[x][y] = 1;
				q.push(Node(x, y));
			}
		}
		q.pop();
	}
}
int main()
{
	while (cin >> n >> m) {
		if (n == 0)
			break;
		init();
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= m; j++) {
				cin >> a[i][j];
				if (a[i][j] == '*')
					visited[i][j] = 1;
			}
		}
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= m; j++) {
				if (a[i][j] == '@' && !visited[i][j]) {
					visited[i][j] = 1;
					cnt++;
					bfs(i, j);
				}
			}
		}
		cout << cnt << endl;
	}
	return 0;
}
