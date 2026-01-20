#include<iostream>
#include<string>
#include<vector>
#include<cstring>
#include<algorithm>
#include<unordered_map>
#include<set>
#include<map>
#include<queue>

using namespace std;

int t, n;

int main() {
	cin >> t;
	while (t--) {
		priority_queue<int,vector<int>,greater<int> >q;
		cin >> n;
		for (int i = 0; i < n; i++) {
			int type, ele;
			cin >> type;
			if (type == 1) {
				cin >> ele;
				q.push(ele);
			}
			else {
				cout << q.top() << endl;
				q.pop();
			}
		}
	}
	return 0;
}