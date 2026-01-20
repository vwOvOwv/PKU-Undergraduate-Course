#include<iostream>
#include<cstring>
#include<algorithm>
#include<map>
#include<queue>

using namespace std;

int n, w;
long long int v[30];
int cnt, mini;
void dfs(int k, vector<long long int>rec) {
	if (k == n + 1) {
		mini = min(mini, cnt);
		return;
	}
	for (int i = 0; i < rec.size(); i++) {
		if (rec[i] + v[k] <= w) {
			rec[i] += v[k];
			dfs(k + 1, rec);
			rec[i] -= v[k];
		}
	}
	if (cnt + 1 < mini) {//²»¼ôÖ¦»á³¬Ê±
		rec.push_back(v[k]);
		cnt++;
		dfs(k + 1, rec);
		rec.erase(--rec.end());
		cnt--;
	}
}
int main() {
	cin >> n >> w;
	for (int i = 1; i <= n; i++)
		cin >> v[i];
	cnt = 0, mini = 1 << 30;
	vector<long long int>rec;
	dfs(1, rec);
	cout << mini;
	return 0;
}
