#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>
using namespace std;

char a[5][5];
vector<pair<int, int> >rec;
int best;
int t;
bool Compare(pair<int, int>& x1, pair<int, int>& x2) {
	if (x1.first < x2.first)
		return true;
	else if (x1.first > x2.first)
		return false;
	else {
		if (x1.second > x2.second)
			return true;
		else
			return false;
	}
}
void dfs(int i, int j) {
	if (i <= 4 && j == 5) {
		dfs(i + 1, 1);
	}
	if (i == 5) {
		for (int i = 1; i <= 4; i++) {
			for (int j = 1; j <= 4; j++) {
				if (a[i][j] == 'W')
					rec[i - 1].first++;
			}
		}
		sort(rec.begin(), rec.end(), Compare);
		for (int i = 0; i < 4; i++) {
			if (rec[i].second == 1) {
				best = min(best, 4 - i);
				break;
			}
		}
		rec.clear();
		for (int i = 1; i < 5; i++)
			rec.push_back(make_pair(0, i));
		return;
	}
	if (a[i][j] == '?') {
		a[i][j] = 'W';
		a[j][i] = 'L';
		dfs(i, j + 1);
		a[i][j] = 'L';
		a[j][i] = 'W';
		dfs(i, j + 1);
		a[i][j] = a[j][i] = '?';//还是得回溯！！！
	}
	if (a[i][j] == 'W' || a[i][j] == 'L' || a[i][j] == '-')
		dfs(i, j + 1);
}
int main()
{
	cin >> t;
	while (t--) {
		rec.clear();
		for (int i = 1; i < 5; i++)
			rec.push_back(make_pair(0, i));
		for (int i = 1; i <= 4; i++) {
			for (int j = 1; j <= 4; j++) {
				cin >> a[i][j];
			}
		}
		best = 5;
		dfs(1, 1);
		cout << best << endl;
	}
	return 0;
}
