#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>
#include<queue>

using namespace std;

int t;
int m, n, h, mini;
char a[265][265];
int visited[265][265][6];
int dx[4] = { 0,1,0,-1 }, dy[4] = { 1,0,-1,0 };
struct Node
{
	int x, y, step, h;
	Node(int xx, int yy, int ss, int hh) :x(xx), y(yy), step(ss), h(hh) {}
};
queue<Node>q;
void init() {
	memset(a, 0, sizeof(a));
	memset(visited, 0, sizeof(visited));
	while (!q.empty())
		q.pop();
}
void bfs() {
	mini = 1 << 30;
	visited[1][1][h] = 1;
	q.push(Node(1, 1, 0, h));
	while (!q.empty()) {
		Node p = q.front();
		if (p.x == m && p.y == n) {
			mini = min(mini, p.step);
		}
		for (int i = 0; i < 4; i++) {
			int x = p.x + dx[i], y = p.y + dy[i];
			if (x >= 1 && x <= m && y >= 1 && y <= n && a[x][y] != '#') {
				if (a[x][y] == '.') {
					if (visited[x][y][p.h] == 0) {
						visited[x][y][p.h] = 1;
						q.push(Node(x, y, p.step + 1, p.h));
					}
				}
				if (a[x][y] == '*') {
					if (visited[x][y][p.h - 1] == 0) {
						visited[x][y][p.h - 1] = 1;
						if (p.h == 1) {
							q.push(Node(1, 1, p.step + 1, h));
						}
						else {
							q.push(Node(x, y, p.step + 1, p.h - 1));
						}
					}
				}
			}
		}
		q.pop();
	}
}
int main()
{
	cin >> t;
	while (t--) {
		cin >> m >> n >> h;
		init();
		for (int i = 1; i <= m; i++) {
			for (int j = 1; j <= n; j++) {
				cin >> a[i][j];
			}
		}
		bfs();
		cout << mini << endl;
	}
	return 0;
}
