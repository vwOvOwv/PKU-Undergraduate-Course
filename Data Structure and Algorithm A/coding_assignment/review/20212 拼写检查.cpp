#include<iostream>
#include<set>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<map>

using namespace std;

vector<string>dic;

int main() {
	string s;
	while (cin >> s) {
		if (s == "#")
			break;
		dic.push_back(s);
	}
	int size = dic.size();
	while (cin >> s) {
		if (s == "#")
			break;
		int l1 = s.size();
		vector<string>rec;
		bool correct = false;
		for (int i = 0; i < size; i++) {
			if (s == dic[i]) {
				correct = true;
				cout << s << " is correct" << endl;
				break;
			}
			int l2 = dic[i].size();
			if (abs(l1 - l2) == 1) {
				int cnt = 0;
				int k1 = 0, k2 = 0;
				while (k1 < l1 && k2 < l2) {
					if (s[k1] != dic[i][k2]) {
						cnt++;
						if (cnt >= 2)
							break;
						if (l1 < l2)
							k2++;
						else
							k1++;
					}
					else
						k1++,k2++;
				}
				if (cnt <= 1)
					rec.push_back(dic[i]);
			}
			else if (l1 == l2) {
				//cout << "called" << endl;
				int cnt = 0;
				for (int j = 0; j < l1; j++) {
					if (s[j] != dic[i][j])
						cnt++;
				}
				if (cnt == 1)
					rec.push_back(dic[i]);
			}
			//cout << rec.back() << endl;
		}
		if (!correct) {
			cout << s << ": ";
			int m = rec.size();
			for (int i = 0; i < m; i++) {
				cout << rec[i];
				if (i < m - 1)
					cout << ' ';
			}
			cout << endl;
		}
	}
	return 0;
}