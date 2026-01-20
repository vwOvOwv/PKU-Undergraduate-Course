#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<iomanip>

using namespace std;

int m, s;
bool flag;
vector<int>rec;
bool IsZero(double a) {
	return abs(a) < 1e-6;
}
void dfs(double sum_force, int left_time,int mini) {
	if (flag)
		return;
	if (IsZero(sum_force - 1.0) && left_time == 0) {
		flag = true;
		cout << rec.size() << ' ';
		for (int i = 0; i < rec.size(); i++) {
			cout << rec[i];
			if (i != rec.size())
				cout << ' ';
		}
		cout << endl;
		return;
	}
	for (int i = mini; i <= left_time; i++) {//最优性剪枝：让结果单调递增
		if (sum_force + 1.0 / i <= 1.0) {//可行性剪枝
			rec.push_back(i);
			dfs(sum_force + 1.0 / i, left_time - i, i);
			rec.erase(--rec.end());
		}
	}
}

int main() {
	cin >> m;
	while (m--) {
		cin >> s;
		rec.clear();
		flag = false;
		dfs(0, s, 1);
		if (!flag)
			cout << "-1" << endl;
	}
	return 0;
}
