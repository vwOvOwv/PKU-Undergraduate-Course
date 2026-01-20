#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<set>

using namespace std;

#define Point pair<int,int>

int main() {
	int n;
	while (cin >> n && n) {
		vector<Point>rec(n);
		set<pair<int,int> >st;
		for (int i = 0; i < n; i++) {
			cin >> rec[i].first>> rec[i].second;
			st.insert(rec[i]);
		}
		int cnt = 0;
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				int x1 = rec[i].first, y1 = rec[i].second;
				int x2 = rec[j].first, y2 = rec[j].second;
				int dx = x2 - x1, dy = y1 - y2;
				Point tmp1, tmp2;
				tmp1.first = x2 + dy, tmp1.second = y2 + dx;
				tmp2.first = x1 + dy, tmp2.second = y1 + dx;
				if (st.count(tmp1) && st.count(tmp2))
					cnt++;
				tmp1.first = x2 - dy, tmp1.second = y2 - dx;
				tmp2.first = x1 - dy, tmp2.second = y1 - dx;
				if (st.count(tmp1) && st.count(tmp2))
					cnt++;
			}
		}
		//cout << cnt << endl;
		cout << cnt / 4 << endl;
	}
}