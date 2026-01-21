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
	int n, k;
	cin >> n >> k;
	int fake = 0;
	int num, animal1, animal2;
	rec.resize(3 * (n + 1));
	for (int i = 1; i <= 3 * n; i++) {
		rec[i].father = i;
		rec[i].weight = 1;
	}
	for (int i = 0; i < k; i++) {
		cin >> num >> animal1 >> animal2;
		if (animal1 > n || animal2 > n || animal1 < 1 || animal2 < 1) {
			fake++;
			continue;
		}
		if (num == 1) {
			if (Find(animal1) == Find(animal2 + n) || Find(animal2) == Find(animal1 + n)) {
				fake++;
				continue;
			}
			Union(animal1, animal2);
			Union(animal1 + n, animal2 + n);
			Union(animal1 + 2 * n, animal2 + 2 * n);
		}
		else {
			if (Find(animal1) == Find(animal2) || Find(animal1 + n) == Find(animal2)) {
				fake++;
				continue;
			}
			Union(animal1, animal2 + n);
			Union(animal1 + n, animal2 + 2 * n);
			Union(animal1 + 2 * n, animal2);
		}
	}
	cout << fake << endl;
}