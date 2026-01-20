#include<iostream>
#include<cstring>
#include<algorithm>
#include<map>
#include<queue>
#include<iomanip>
#include<stack>

using namespace std;

int n, m, k,sx, sy;
vector<pair<int, int>>rec;
char a[110][110];
bool visited[110][110][10][1 << 7];
int dx[4] = { 0,1,0,-1 }, dy[4] = { 1,0, -1,0 };
struct Node
{
	int x, y, now, step, sit;
	Node(int xx, int yy, int nn, int ss,int s) :x(xx), y(yy), now(nn), step(ss),sit(s) {}
};
queue<Node>q;

int main() {
	cin >> n >> m;
	int tmp = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			if (a[i][j] == 'S') {
				sx = i, sy = j;
			}
			if (a[i][j] == 'G') {
				rec.push_back(make_pair(i, j));
				int s = rec.size() - 1;
				tmp |= 1 << s;
			}
			if (a[i][j] >= '1' && a[i][j] <= '9') {
				k = max(k, a[i][j] - '0');
			}
		}
	}
	q.push(Node(sx, sy, 0, 0, tmp));
	visited[sx][sy][0][tmp] = 1;
	int mini = 1 << 30;
	while (!q.empty()) {
		Node p = q.front();
		if (a[p.x][p.y] == 'T' && p.now == k) {
			mini = min(mini, p.step);
			break;
		}
		if (a[p.x][p.y] == 'G') {
			int ii;
			for (ii = 0; ii < rec.size(); ii++) {
				if (rec[ii].first == p.x && rec[ii].second == p.y) {
					break;
				}
			}
			if (!visited[p.x][p.y][p.now][p.sit & ~(1 << ii)]) {
				visited[p.x][p.y][p.now][p.sit & ~(1 << ii)] = 1;
				q.push(Node(p.x, p.y, p.now, p.step + 1, p.sit & ~(1 << ii)));
				q.pop();
				continue;
			}
		}
		for (int i = 0; i < 4; i++) {
			int x = p.x + dx[i], y = p.y + dy[i];
			if (x >= 1 && x <= n && y >= 1 && y <= m) {
				if (a[x][y] == 'X')
					continue;
				if (a[x][y] == 'G' || a[x][y] == '.' || a[x][y] == 'T' || a[x][y] == 'S') {
					if (!visited[x][y][p.now][p.sit]) {
						visited[x][y][p.now][p.sit] = 1;
						q.push(Node(x, y, p.now, p.step + 1, p.sit));
					}
				}
				if (a[x][y] >= '1' && a[x][y] <= '9') {
					if (a[x][y] - '0' == p.now + 1) {
						visited[x][y][p.now + 1][p.sit] = 1;
						q.push(Node(x, y, p.now + 1, p.step + 1, p.sit));
					}
					else if (!visited[x][y][p.now][p.sit]) {
						visited[x][y][p.now][p.sit] = 1;
						q.push(Node(x, y, p.now, p.step + 1, p.sit));
					}
				}
			}
		}
		q.pop();
	}
	cout << mini << endl;
}
