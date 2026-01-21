#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>

using namespace std;

int t;
int n, m;
char a[110][110];
int visited[110][110][2];
int sx, sy, ex, ey;
int dx[4] = { 0,1,0,-1 }, dy[4] = { 1,0,-1,0 };
bool flag;
int cnt;

struct Dot {
	int x, y;
	Dot(int xx, int yy) :x(xx), y(yy) {}
};
vector< pair<Dot, Dot> > rec(30, make_pair(Dot(-1, -1), Dot(-1, -1)));
struct Node {
	int x, y, t;
	Node(int xx, int yy, int tt) :x(xx), y(yy), t(tt) {}
};
queue<Node>q;

void init() {
	memset(a, 0, sizeof(a));
	memset(visited, 0, sizeof(visited));
	while (!q.empty())
		q.pop();
	flag = false;
	for (int i = 0; i < rec.size(); i++) {
		rec[i].first.x = -1;
		rec[i].first.y = -1;
		rec[i].second.x = -1;
		rec[i].second.y = -1;
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			if (a[i][j] == 'B')
				sx = i, sy = j, visited[i][j][0] = 0;
			else if (a[i][j] == 'I')
				ex = i, ey = j, visited[i][j][0] = 1;
			else if (a[i][j] >= 'a' && a[i][j] <= 'z') {
				visited[i][j][0] = 1;
				visited[i][j][1] = 1;
				if (rec[a[i][j] - 'a'].first.x == -1) {//初始化要重置rec！！！
					rec[a[i][j] - 'a'].first.x = i;
					rec[a[i][j] - 'a'].first.y = j;
				}
				else {
					rec[a[i][j] - 'a'].second.x = i;
					rec[a[i][j] - 'a'].second.y = j;
				}
			}
			else if (a[i][j] == '#')
				visited[i][j][0] = 0;
			else
				visited[i][j][0] = 1;
		}
	}
	cnt++;
	cout << "Case #" << cnt << ": ";
}
void bfs() {
	q.push(Node(sx, sy, 0));
	while (!q.empty()) {
		Node p = q.front();
		if (p.x == ex && p.y == ey) {
			flag = true;
			cout << p.t << endl;
			break;
		}
		if (a[p.x][p.y] >= 'a' && a[p.x][p.y] <= 'z' && visited[p.x][p.y][1]) {
			if (p.x == rec[a[p.x][p.y] - 'a'].first.x && p.y == rec[a[p.x][p.y] - 'a'].first.y) {
				q.push(Node(rec[a[p.x][p.y] - 'a'].second.x, rec[a[p.x][p.y] - 'a'].second.y, p.t + 1));
				visited[p.x][p.y][1] = 0;
				//visited[rec[a[p.x][p.y] - 'a'].second.x][rec[a[p.x][p.y] - 'a'].second.y][0] = 0;
			}
			else {
				q.push(Node(rec[a[p.x][p.y] - 'a'].first.x, rec[a[p.x][p.y] - 'a'].first.y, p.t + 1));
				visited[p.x][p.y][1] = 0;
				//visited[rec[a[p.x][p.y] - 'a'].first.x][rec[a[p.x][p.y] - 'a'].first.y][0] = 0;
			}
		}
		for (int i = 0; i < 4; i++) {
			int x = p.x + dx[i], y = p.y + dy[i];
			if (x >= 1 && x <= n && y >= 1 && y <= m) {
				if (visited[x][y][0]) {
					visited[x][y][0] = 0;
					q.push(Node(x, y, p.t + 1));
				}
			}
		}
		q.pop();
	}
	if (!flag)
		cout << "-1" << endl;
}
int main()
{
	cin >> t;
	while (t--) {
		cin >> n >> m;
		init();
		bfs();
	}
	return 0;
}
