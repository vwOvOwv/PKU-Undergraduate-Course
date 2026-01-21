#include<iostream>
#include<cstring>
#include<algorithm>
#include<map>
#include<queue>

using namespace std;

int n;
vector<pair<int, int>>cir;
int maxi;

int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		int a, b;
		cin >> a >> b;
		cir.push_back(make_pair(a, b));
	}
	for (int i = 0; i < cir.size(); i++) {
		for (int j = 0; j < cir.size(); j++) {
			if (i == j)
				continue;
			if (min(cir[i].second, cir[j].second) - max(cir[i].first, cir[j].first)+1 > 0)
				maxi = max(maxi, min(cir[i].second, cir[j].second) - max(cir[i].first, cir[j].first) + 1);
		}
	}
	cout << maxi << endl;
	return 0;
}
