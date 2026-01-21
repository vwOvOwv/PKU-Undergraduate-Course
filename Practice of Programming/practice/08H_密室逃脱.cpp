#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<cstring>
#include<map>

using namespace std;
#define mem(a) memset(a,0,sizeof(a))

//注意本题未提及最多线索数量，最多可能有9999个线索，故三维visited数组判重会mle，所以采用分解为多次bfs的方法判重
int t, m, n;
int a[110][110];
bool visited[110][110];
int dx[4] = { 0,0,1,-1 }, dy[4] = { 1,-1,0,0 };
struct Node
{
	int x, y, s;
};
queue<Node>q;
map<int, pair<int, int>>hint;
void init() {
	mem(a), mem(visited);
	while (!q.empty())
		q.pop();
	hint.clear();
}
int bfs(int i,int j,int x,int y) {
	while (!q.empty())
		q.pop();
	mem(visited);
	q.push({ i,j,0 });
	while(!q.empty()) {
		Node p = q.front();
		if (p.x == x && p.y == y) {
			return p.s;
		}
		for (int i = 0; i < 4; i++) {
			int xx = p.x + dx[i], yy = p.y + dy[i];
			if (xx >= 1 && xx <= m && yy >= 1 && yy <= n && a[xx][yy] != 0 && !visited[xx][yy]) {
				visited[xx][yy] = 1;
				q.push({ xx,yy,p.s + 1 });
			}
		}
		q.pop();
	}
	return -1;
}

int main() {
	cin >> t;
	while (t--) {
		cin >> m >> n;
		init();
		for (int i = 1; i <= m; i++) {
			for (int j = 1; j <= n; j++) {
				cin >> a[i][j];
				if (a[i][j] > 1)
					hint.insert(make_pair(a[i][j], make_pair(i, j)));
			}
		}
		int cnt = 0;
		int si = 1, sj = 1;
		for (auto i:hint) {
			int tmp = bfs(si, sj, i.second.first, i.second.second);
			if (tmp == -1) {
				cout << "-1" << endl;
				goto loop;
			}
			cnt += tmp;
			si = i.second.first, sj = i.second.second;
		}
		cout << cnt << endl;
	loop:;
	}
	return 0;
}
