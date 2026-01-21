#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>

using namespace std;
#define mem(a) memset(a,0,sizeof(a))
//这个题不能简单地用vis判重的，没办法做到按顺序一层一层的搜，比如多次连跳就要一直向下搜，很难实现
//所以要记录下到达每个状态需要的时间才可以
int n, m, v, mini, testCase;
char a[110][110];
int dis[110][110][4][11];//记录各状态到起始状态的距离
int dx[4] = { 0,0,1,-1 }, dy[4] = { 1,-1,0,0 };
struct Node
{
	int x, y, dir, bat, t;
};
queue<Node>q;

void init() {
	mem(a);
	memset(dis, 0x3f, sizeof(dis));
	while (!q.empty())
		q.pop();
	mini = 1 << 30;
}
void bfs() {
	while (!q.empty()) {
		Node p = q.front();
		if (a[p.x][p.y] == 'T')
			mini = min(mini, p.t);
		if (a[p.x][p.y] == 'E') {//先考虑可以跳的情况
			for (int j = 1; j <= p.bat; j++) {
				int x = p.x + dx[p.dir] * j;
				int y = p.y + dy[p.dir] * j;
				int d = dis[p.x][p.y][p.dir][p.bat];
				x = max(1, x), x = min(x, n);//挡住了
				y = max(1, y), y = min(y, m);
				if (a[x][y] != '#' && dis[x][y][p.dir][j] > d) {//可以更快地到达该状态
					q.push({ x,y,p.dir,j,d });//注意更新的时候不能用p.t（为什么？？？）
					dis[x][y][p.dir][j] = d;//该状态重新入队，因为该点后续的状态都可能有更优的路径
				}
			}
		}
		for (int i = 0; i < 4; i++) {//再考虑正常走
			int x = p.x + dx[i], y = p.y + dy[i];
			int d = dis[p.x][p.y][p.dir][p.bat] + 1;
			if (x >= 1 && x <= n && y >= 1 && y <= m) {
				if (a[x][y] != '#') {
					if (dis[x][y][i][p.bat] > d) {//有更优路径
						q.push({ x,y,i,p.bat,d });
						dis[x][y][i][p.bat] = d;
					}
				}
			}
		}
		q.pop();
	}
	cout << "Case #" << ++testCase << ": ";
	if (mini == 1 << 30)
		cout << "-1" << endl;
	else
		cout << mini << endl;
}

int main() {
	while (cin >> n >> m >> v && n) {
		init();
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= m; j++) {
				cin >> a[i][j];
				if (a[i][j] == 'S') {
					q.push({ i,j,0,v,0 });//起点方向随意
					dis[i][j][0][v] = dis[i][j][1][v] = dis[i][j][2][v] = dis[i][j][3][v] = 0;
				}
			}
		}
		bfs();
	}
	return 0;
}
