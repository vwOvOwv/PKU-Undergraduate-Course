#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

class TreeNode {
public:
	int father,weight;
	TreeNode() {}
	TreeNode(int f, int w) :father(f), weight(w) {}
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
	int t;
	cin >> t;
	while (t--) {
		int n, m;
		cin >> n >> m;
		rec.resize(2 * (n + 1));
		for (int i = 1; i <= 2 * n; i++) {
			rec[i].father = i;
			rec[i].weight = 1;
		}
		char ch;
		int crime1, crime2;
		for (int i = 0; i < m; i++) {
			cin >> ch >> crime1 >> crime2;
			if (ch == 'A') {
				if (Find(crime1) == Find(crime2))
					cout << "In the same gang." << endl;
				else if (Find(crime1 + n) == Find(crime2))
					cout << "In different gangs." << endl;
				else
					cout << "Not sure yet." << endl;
			}
			else {
				Union(crime1 + n, crime2);
				Union(crime1, crime2 + n);
			}
		}
	}
}