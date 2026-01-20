#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#define mem(a) memset(a,0,sizeof(a))
using namespace std;

int t, r, c, k;
char a[110][110];
int visited[110][110][15];
int dx[4] = { 0,1,0,-1 }, dy[4] = { 1,0,-1,0 };

struct Node
{
 int x, y, t;
 Node(int xx, int yy, int tt) :x(xx), y(yy), t(tt) {}
};
queue<Node>q;

void init() {
 mem(visited);
 mem(a);
 while (!q.empty())
  q.pop();
}
void bfs() {
 while (!q.empty()) {
  Node p = q.front();
  if (a[p.x][p.y] == 'E') {
   cout << p.t << endl;
   return;
  }
  for (int i = 0; i < 4; i++) {
   int x = p.x + dx[i], y = p.y + dy[i];
   if (x >= 1 && x <= r && y >= 1 && y <= c && visited[x][y][(p.t + 1) % k] == 0) {
    if (a[x][y] == '.' || a[x][y] == 'E') {//'S'ÄØ£¿£¿£¿
     visited[x][y][(p.t + 1) % k] = 1;
     q.push(Node(x, y, p.t + 1));
    }
    else if (a[x][y] == '#' && (p.t + 1) % k == 0) {
     visited[x][y][(p.t + 1) % k] = 1;
     q.push(Node(x, y, p.t + 1));
    }
   }
  }
  q.pop();
 }
 cout << "Oop!" << endl;
}

int main()
{
 cin >> t;
 while (t--) {
  cin >> r >> c >> k;
  init();
  for (int i = 1; i <= r; i++) {
   for (int j = 1; j <= c; j++) {
    cin >> a[i][j];
    if (a[i][j] == 'S') {
     visited[i][j][0] = 1;
     q.push(Node(i, j, 0));
    }
   }
  }
  bfs();
 }
 return 0;
}
