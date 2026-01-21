#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<cstring>
#include<map>

using namespace std;

int n;
int a[310][310];
int visited[310][310];
int dx[4] = { 1,0,-1,0 }, dy[4] = { 0,1,0,-1 };

struct Node
{
	int x, y;
};

int bfs(int t) {
	if (t < a[1][1])
		return -1;
	queue<Node>q;
	memset(visited, 0, sizeof(visited));
	q.push({ 1,1 });
	visited[1][1] = 1;
	while (!q.empty()) {
		Node p = q.front();
		if (p.x == n && p.y == n)
			return 1;
		for (int i = 0; i < 4; i++) {
			int x = p.x + dx[i], y = p.y + dy[i];
			if (x >= 1 && x <= n && y >= 1 && y <= n && t >= a[x][y]) {
				if (!visited[x][y]) {
					visited[x][y] = 1;
					q.push({ x,y });
				}
			}
		}
		q.pop();
	}
	return -1;
}

int main() {
	cin >> n;
	int maxi = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> a[i][j];
			maxi = max(maxi, a[i][j]);
		}
	}
	int L = 0, R = maxi, ans;
	while (L <= R) {//枚举时间，用二分查找降低复杂度，否则tle
		int mid = L + (R - L) / 2;
		int t = bfs(mid);
		if (t == -1)
			L = mid + 1;
		else {
			ans = mid;
			R = mid - 1;
		}
	}
	cout << ans << endl;
	return 0;
}
