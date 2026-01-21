#include<iostream>
#include<cstring>
#include<algorithm>
#include<map>
#include<queue>

using namespace std;

map<string, int>mem;
int visited[100000][4][3];//为什么要这样设定状态？
struct Node
{
	string num;
	int step, cnt2, cnt3;
	Node(string nn, int ss, int c2, int c3) :num(nn), step(ss), cnt2(c2), cnt3(c3) {}
};
queue<Node>q;
void bfs() {
	q.push(Node("12345", 0, 0, 0));
	mem["12345"] = -1;
	while (!q.empty()) {
		Node p = q.front();
		for (int i = 0; i < 4; i++) {//交换
			string tmp = p.num;
			swap(tmp[i], tmp[i + 1]);
			if (!visited[atoi(tmp.c_str())][p.cnt2][p.cnt3]) {
				q.push(Node(tmp, p.step + 1, p.cnt2, p.cnt3));
				if (mem[tmp] == 0)
					mem[tmp] = p.step + 1;
				else
					mem[tmp] = min(mem[tmp], p.step + 1);
				visited[atoi(tmp.c_str())][p.cnt2][p.cnt3] = 1;
			}
		}
		if (p.cnt2 + 1 <= 3) {//加1
			for (int i = 0; i < 5; i++) {
				string tmp = p.num;
				tmp[i]++;
				if (tmp[i] - '0' >= 10)
					tmp[i] = '0';
				if (!visited[atoi(tmp.c_str())][p.cnt2 + 1][p.cnt3]) {
					q.push(Node(tmp, p.step + 1, p.cnt2 + 1, p.cnt3));
					if (mem[tmp] == 0)
						mem[tmp] = p.step + 1;
					else
						mem[tmp] = min(mem[tmp], p.step + 1);
					visited[atoi(tmp.c_str())][p.cnt2 + 1][p.cnt3] = 1;
				}
			}
		}
		if (p.cnt3 + 1 <= 2) {//加倍
			for (int i = 0; i < 5; i++) {
				string tmp = p.num;
				int t = 2 * (tmp[i] - '0');
				if (t >= 10)
					t %= 10;
				tmp[i] = t + '0';
				if (!visited[atoi(tmp.c_str())][p.cnt2][p.cnt3 + 1]) {
					q.push(Node(tmp, p.step + 1, p.cnt2, p.cnt3 + 1));
					if (mem[tmp] == 0)
						mem[tmp] = p.step + 1;
					else
						mem[tmp] = min(mem[tmp], p.step + 1);
					visited[atoi(tmp.c_str())][p.cnt2][p.cnt3 + 1] = 1;
				}
			}
		}
		q.pop();
	}
}
int main()
{

	bfs();
	string s;
	while (cin >> s) {
		if (s == "12345")
			cout << 0 << endl;
		else if (mem[s] == 0)
			cout << "-1" << endl;
		else
			cout << mem[s] << endl;
	}
	return 0;
}
