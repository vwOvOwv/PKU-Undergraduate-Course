#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;

int h, w, m;
int a[60][60];
bool visited[60][60];
int cnt;
int dx[4] = { 0,1,0,-1 }, dy[4] = { 1,0,-1,0 };

struct Node
{
	int x, y;
	Node(int xx, int yy) :x(xx), y(yy) {}
};
queue<Node>q;
void init() {
	memset(a, 0, sizeof(a));
	memset(visited, 0, sizeof(visited));
	cnt = 0;
	while (!q.empty())
		q.pop();
}
void bfs() {
	while (!q.empty()) {
		Node p = q.front();
		for (int i = 0; i < 4; i++) {
			int x = p.x + dx[i], y = p.y + dy[i];
			if (x >= 1 && x <= h && y >= 1 && y <= w && !visited[x][y] && abs(a[x][y] - a[p.x][p.y]) <= m) {
				visited[x][y] = 1;
				q.push(Node(x, y));
			}
		}
		q.pop();
	}
}

int main() {
	while (cin >> h >> w >> m && h) {
		init();
		for (int i = 1; i <= h; i++) {
			for (int j = 1; j <= w; j++) {
				cin >> a[i][j];
			}
		}
		for (int i = 1; i <= h; i++) {
			for (int j = 1; j <= w; j++) {
				if (!visited[i][j]) {
					cnt++;
					q.push(Node(i, j));
					visited[i][j] = 1;
					bfs();
				}
			}
		}
		cout << cnt << endl;
	}
}
