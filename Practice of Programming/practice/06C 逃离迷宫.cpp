#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>
#include<queue>

using namespace std;

int k, t, m;
char a[20][20];
int visited[20][20];
int dx[4] = { 0,1,0,-1 }, dy[4] = { 1,0,-1,0 };
struct Node
{
	int x, y, t;
	Node(int xx, int yy, int tt) :x(xx), y(yy), t(tt) {}
};
queue<Node>q;

int main()
{
	cin >> k;
	while (k--) {
		cin >> m >> t;
		memset(a, 0, sizeof(a));
		memset(visited, 0, sizeof(visited));
		while (!q.empty())//if?2022.6.15
			q.pop();
		for (int i = 1; i <= m; i++) {
			for (int j = 1; j <= m; j++) {
				cin >> a[i][j];
				if (a[i][j] == '#')
					visited[i][j] = 1;
				if (a[i][j] == 'S') {
					visited[i][j] = 1;
					q.push(Node(i, j, 0));
				}
			}
		}
		while (!q.empty()) {
			Node p = q.front();
			if (a[p.x][p.y] == 'E') {
				if (p.t <= t)
					cout << "YES" << endl;
				else
					cout << "NO" << endl;
				break;
			}
			for (int k = 0; k < 4; k++) {
				int x = p.x + dx[k], y = p.y + dy[k];
				if (x >= 1 && x <= m && y >= 1 && y <= m && visited[x][y] == 0) {
					q.push(Node(x, y, p.t + 1));
					visited[x][y] = 1;
				}
			}
			q.pop();
		}
		if (q.empty())
			cout << "NO" << endl;
	}
	return 0;
}
