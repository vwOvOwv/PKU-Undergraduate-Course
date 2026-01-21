//经典的bfs
#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<climits>
#include<vector>
using namespace std;

int t;
int n;
int a[20][20];
bool ans;
int visited[20];
struct Dot
{
	int id;
	Dot(int k) :id(k) {}
};
queue<Dot>q;
int main()
{
	cin >> t;
	while (t--) {
		cin >> n;
		memset(a, 0, sizeof(a));
		memset(visited, 0, sizeof(visited));
		while (!q.empty())
			q.pop();
		ans = false;
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				cin >> a[i][j];
			}
		}
		q.push(Dot(1));
		visited[1] = 1;
		while (!q.empty()) {
			Dot p = q.front();
			if (p.id == n) {
				ans = true;
				break;
			}
			for (int i = 1; i <= n; i++) {
				if (p.id == i)
					continue;
				if (a[p.id][i] > 0&&visited[i]==0) {
					visited[i] = 1;
					q.push(Dot(i));
				}
			}
			q.pop();
		}
		if (ans)
			cout << "YES" << endl;
		else
			cout << "NO" << endl;
	}
	return 0;
}
