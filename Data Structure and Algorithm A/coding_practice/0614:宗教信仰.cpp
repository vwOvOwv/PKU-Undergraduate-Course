#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

class TreeNode {
public:
	int father, weight;
};

vector<TreeNode>rec;

int Find(int i) {
	if (rec[i].father == i)
		return i;
	rec[i].father = Find(rec[i].father);
	return rec[i].father;
}

void Union(int i, int j) {
	int rt1 = Find(i), rt2 = Find(j);
	//cout << "i=" << i << ' ' << "j=" << j << endl;
	//cout << "rt1=" << rt1 << ' ' << "rt2=" << rt2 << endl;
	if (rt1 != rt2) {
		if (rec[rt1].weight < rec[rt2].weight) {
			rec[rt1].father = rt2;
			rec[rt2].weight += rec[rt1].weight;
		}
		else {
			rec[rt2].father = rt1;
			rec[rt1].weight += rec[rt2].weight;
		}
	}
}

int main() {
	int n, m;
	int t = 0;
	while (cin >> n >> m && n && m) {
		t++;
		rec.resize(n + 1);
		for (int i = 1; i <= n; i++) {
			rec[i].father = i;
			rec[i].weight = 1;
		}
		int equlNum = 0;
		int stu1, stu2;
		for (int i = 0; i < m; i++) {
			cin >> stu1 >> stu2;
			Union(stu1, stu2);
		}
		/*for (int i = 1; i <= n; i++) {
			cout << rec[i].father << ' ';
		}
		cout << endl;*/
		for (int i = 1; i <= n; i++) {
			if (rec[i].father == i)
				equlNum++;
		}
		cout << "Case " << t << ": " << equlNum << endl;
	}
}