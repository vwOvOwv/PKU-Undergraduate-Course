#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<climits>

using namespace std;

int a, b, c;
int visited[110][110];

struct Dot
{
	int id1;
	int id2;
	int v1;
	int v2;
	string op;
	int prev;
	int step;
	Dot(int i1,int i2, int vv1,int vv2, string oo, int pp, int ss) :id1(i1),id2(i2), v1(vv1),v2(vv2), op(oo), prev(pp), step(ss) {}
};
vector<Dot>q;
int head = 0, tail = 0;
bool empty() {
	if (head == tail)
		return true;
	else
		return false;
}
void push(Dot x) {
	q.push_back(x);
	tail++;
}
Dot& front() {
	return q[head];
}
void pop() {
	head++;
}
string fill(int id) {
	string ans = "FILL(";
	ans.push_back(id + '0');
	ans += ")";
	return ans;
}
string pour(int id1, int id2) {
	string ans = "POUR(";
	ans.push_back(id1 + '0');
	ans += ",";
	ans.push_back(id2 + '0');
	ans += ")";
	return ans;
}
string drop(int id) {
	string ans = "DROP(";
	ans.push_back(id + '0');
	ans += ")";
	return ans;
}
int main()
{
	cin >> a >> b >> c;
	int mini = INT_MAX;
	bool flag = false;
	visited[0][0] = 1;
	vector<string>ans;
	push(Dot(1, 2, 0, 0, "null", -1, 0));
	while (!empty()) {
		Dot p = front();
		if (p.v1 == c || p.v2 == c) {
			flag = true;
			mini = min(mini, p.step);
			if (mini == p.step) {
				ans.clear();
				int pre = p.prev;
				ans.push_back(p.op);
				while (pre != 0) {
					ans.push_back(q[pre].op);
					pre = q[pre].prev;
				}
			}
		}
		for (int i = 0; i < 6; i++) {
			switch (i)
			{
				int t1, t2;
			case 0:
				t1 = p.v1;
				t2 = p.v2;
				if (t1 >= b - t2) {
					t1 = t1 - (b - t2);
					t2 = b;
				}
				else {
					t2 += t1;
					t1 = 0;
				}
				if (visited[t1][t2] == 0) {
					visited[t1][t2] = 1;
					push(Dot(1, 2, t1, t2, pour(1, 2), head, p.step + 1));
				}
				break;
			case 1:
				t1 = p.v1;
				t2 = p.v2;
				if (t2 >= a - t1) {
					t2 = t2 - (a - t1);
					t1 = a;
				}
				else {
					t1 += t2;//注意这两步不要写反了
					t2 = 0;
				}
				if (visited[t1][t2] == 0) {
					visited[t1][t2] = 1;
					push(Dot(1, 2, t1, t2, pour(2, 1), head, p.step + 1));
				}
				break;
			case 2:
				if (visited[0][p.v2] == 0) {
					visited[0][p.v2] = 1;
					push(Dot(1, 2, 0, p.v2, drop(1), head, p.step + 1));
				}
				break;
			case 3:
				if (visited[p.v1][0] == 0) {
					visited[p.v1][0] = 1;
					push(Dot(1, 2, p.v1, 0, drop(2), head, p.step + 1));
				}
				break;
			case 4:
				if (visited[a][p.v2] == 0) {
					visited[a][p.v2] = 1;
					push(Dot(1, 2, a, p.v2, fill(1), head, p.step + 1));
				}
				break;
			case 5:
				if (visited[p.v1][b] == 0) {
					visited[p.v1][b] = 1;
					push(Dot(1, 2, p.v1, b, fill(2), head, p.step + 1));
				}
				break;
			}
		}
		pop();
	}
	if (!flag)
		cout << "impossible" << endl;
	else {
		cout << mini << endl;
		for (int i = ans.size() - 1; i >= 0; i--)//注意输出顺序！！！
			cout << ans[i] << endl;
	}
	return 0;
}
