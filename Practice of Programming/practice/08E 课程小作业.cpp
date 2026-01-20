#include<iostream>
#include<vector>
#include<queue>
using namespace std;

vector<int> ddls[10001];
priority_queue<int, vector<int>, less<int> > works;
//朴素贪心
int main() {
	int n;
	cin >> n;
	int s = 0;
	while (n--) {
		int score, ddl;
		cin >> score >> ddl;
		ddls[ddl].push_back(score);
	}
	for (int i = 10000; i > 0; --i) {
		for (int score : ddls[i])
			works.push(score);//分高的、时间靠后的先做
		if (!works.empty()) {
			s += works.top();
			works.pop();
		}
	}
	cout << s << endl;
	return 0;
}

