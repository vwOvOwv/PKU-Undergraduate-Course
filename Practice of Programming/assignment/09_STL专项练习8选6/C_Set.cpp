#include <iostream>
#include <set>
#include <string>
#include<algorithm>
using namespace std;

typedef multiset<int> arr;
arr a;
multiset<int>::iterator i;
int flag[10000] = { 0 };//标记是否被添加过
void add(int m);
void del(int m);
void ask(int m);

int main()
{

	int n;
	int x;
	cin >> n;
	string cmd;
	while (n--) {
		cin >> cmd >> x;
		if (cmd == "add") {
			add(x);
		}
		else if (cmd == "del") {
			del(x);
		}
		else if (cmd == "ask") {
			ask(x);
		}
	}
	return 0;
}
void add(int m) {
	a.insert(m);
	flag[m] = 1;//改变标记状态
	int cnt = 0;
	for (i = a.begin(); i != a.end(); i++) {
		if (*i == m)
			cnt++;
	}
	cout << cnt << endl;
}
void del(int m) {
	int cnt = 0;
	for (i = a.begin(); i != a.end(); i++) {
		if (*i == m)
			cnt++;
	}
	cout << cnt << endl;
	a.erase(m);
}
void ask(int m) {
	int cnt = 0;
	for (i = a.begin(); i != a.end(); i++) {
		if (*i == m)
		{
			cnt++;
		}
	}
	cout << flag[m] << ' ';
	cout << cnt << endl;
}
