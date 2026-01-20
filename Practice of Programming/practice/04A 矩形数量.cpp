#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<climits>
#include<vector>
using namespace std;

int t, n;


vector<pair<int,int> >v;
bool isRectangle(pair<int,int> a1, pair<int,int> a2, pair<int,int> a3, pair<int,int> a4) {
	if (a1.first == a2.first && a1.second == a3.second && a2.second == a4.second && a3.first == a4.first)
		return true;
	else
		return false;
}

int main()
{
	cin >> t;
	while (t--) {
		v.clear();
		cin >> n;
		int cnt = 0;
		for (int i = 0; i < n; i++) {
			int x, y;
			cin >> x >> y;
			v.push_back(make_pair(x, y));
		}
		sort(v.begin(), v.end());//排序很重要，关系到unique与isRectangle函数
		auto ii=unique(v.begin(), v.end());//去掉重复点
		auto jj = v.begin();
		int size = 0;
		for (jj = v.begin(); jj != ii; ++jj)
			size++;
		for (int a = 0; a < size; a++) {
			for (int b = a + 1; b < size; b++) {
				for (int c = b + 1; c < size; c++) {
					for (int d = c + 1; d < size; d++) {
						if (isRectangle(v[a], v[b], v[c], v[d])) {
							cnt++;
						}
					}
				}
			}
		}
		cout << cnt << endl;
	}
	return 0;
}
