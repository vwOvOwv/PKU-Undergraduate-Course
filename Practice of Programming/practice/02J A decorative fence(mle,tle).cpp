#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>

using namespace std;

int k, n, c;
vector<vector<int> >v;
vector<int>flag(30);
vector<int>rec(30, 1 << 30);
bool Compare(const vector<int>& v1, const vector<int>& v2) {
	int i;
	for (i = 1; i < v1.size(); i++) {
		if (v1[i] == 1 << 30)
			return true;
		if (v1[i] < v2[i])
			return true;
		else if (v1[i] > v2[i])
			return false;
	}
}
void dfs(int i) {
	if (i == n + 1) {
		v.push_back(rec);
		return;
	}
	if (i == 1) {
		for (int j = 1; j <= n; j++) {
			flag[j] = 1;rec[0] = 1 << 30;rec[1] = j;
			dfs(i + 1);
			rec[1] = 0;rec[0] = 0;flag[j] = 0;
			flag[j] = 1; rec[0] = 0; rec[1] = j;
			dfs(i + 1);
			flag[j] = 0; rec[0] = 0; rec[1] = 0;
		}
	}
	else {
		for (int j = 1; j <= n; j++) {
			if (!flag[j]) {
				if ((rec[i - 1] > rec[i - 2] && j < rec[i - 1])||(rec[i - 1]<rec[i - 2] && j>rec[i - 1])) {
					flag[j] = 1;
					rec[i] = j;
					dfs(i + 1);
					rec[i] = 0;
					flag[j] = 0;
				}
			}
		}
	}
}

int main()
{
	cin >> k;
	while (k--) {
		cin >> n >> c;
		v.clear();
		for (int i = 0; i < 30; i++) {
			rec[i] = 1 << 30;
			flag[i] = 0;
		}

		dfs(1);
		if (n > 1)
			sort(v.begin(), v.end(), Compare);
		for (int i = 1; i < v[c - 1].size(); i++) {
			if (v[c - 1][i] == 1 << 30)
				break;
			cout << v[c - 1][i];
			if (v[c - 1][i + 1] != 1 << 30)
				cout << ' ';
		}
		cout << endl;
	}
	return 0;
}
