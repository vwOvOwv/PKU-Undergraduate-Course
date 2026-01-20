#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>

using namespace std;

int cnt_r[110], cnt_c[110];
int n;
int a[110][110];
bool flag;
vector<int>rec_r, rec_c;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> a[i][j];
			if (a[i][j] == 1) {
				cnt_r[i]++;
				cnt_c[j]++;
			}
		}
	}
	flag = true;
	for (int i = 1; i <= n; i++) {
		if (cnt_r[i] % 2 != 0 || cnt_c[i] % 2 != 0) {
			flag = false;
			break;
		}
	}
	if (flag) {
		cout << "OK" << endl;
		return 0;
	}
	for (int i = 1; i <= n; i++) {
		if (cnt_r[i] % 2 != 0)
			rec_r.push_back(i);
		if (cnt_c[i] % 2 != 0)
			rec_c.push_back(i);
	}
	if (rec_r.size() == 1 && rec_c.size() == 1) {
		cout << rec_r[0] << ' ' << rec_c[0] << endl;
		return 0;
	}
	else {
		cout << "Corrupt" << endl;
		return 0;
	}
	return 0;
}
