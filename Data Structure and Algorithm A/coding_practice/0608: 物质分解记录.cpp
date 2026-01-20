#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

void solve(vector<string>rec, string target) {
	vector<pair<string, int> >ans;
	int n = rec.size();
	int cur = 0;
	for (int i = 0; i < n; i++) {
		if (rec[i] == "{")
			cur++;
		else if (rec[i] == "}")
			cur--;
		else {
			ans.push_back(make_pair(rec[i], cur));
			//cout << rec[i] << ' ' << cur << endl;
		}
	}
	int size = ans.size();
	int targetDepth = -1;
	int i;
	for (i = 0; i < size; i++) {
		if (ans[i].first == target) {
			targetDepth = ans[i].second;
			break;
		}
	}
	if (i == size)
		cout << "No" << endl;
	vector<string>trueAns;
	for (int j = i + 1; j < size; j++) {
		if (ans[j].second == targetDepth)
			trueAns.push_back(ans[j].first);
		if (ans[j].second < targetDepth)
			break;
	}
	int m = trueAns.size();
	if (!m)
		cout << "";
	else {
		for (int i = 0; i < m; i++) {
			cout << trueAns[i];
		}
		cout << endl;
	}
}

int main() {
	int n;
	cin >> n;
	while (n--) {
		string str="/";
		vector<string>rec;
		cin.get();
		while (1) {
			getline(cin, str);
			//cout << strlen(str) << endl;
			//cout << "str=" << str << endl;//
			if (str=="") {
				//cout << "called" << endl;
				break;
			}
			rec.push_back(str);
		}
		//for (auto i:rec) {
		//	cout << i << endl;
		//}
		string target;
		cin >> target;

		solve(rec, target);

		getline(cin, str);
		getline(cin, str);
	}
}