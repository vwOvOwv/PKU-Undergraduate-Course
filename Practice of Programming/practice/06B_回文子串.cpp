#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>

using namespace std;

int n;
string s;
bool judge(string s) {
	for (int i = 0, j = s.length() - 1; i <= j; j--, i++) {
		if (s[i] != s[j])
			return false;
	}
	return true;
}
void solve() {
	int maxlen = s.length();
	for (int l = maxlen; l >= 1; l--) {
		for (int i = 0; i <= s.length() - l; i++) {
			string tmp = s.substr(i, l);
			if (judge(tmp)) {
				cout << tmp << endl;
				return;
			}
		}
	}
}

int main()
{
	cin >> n;
	while (n--) {
		cin >> s;
		solve();
	}
	return 0;
}
