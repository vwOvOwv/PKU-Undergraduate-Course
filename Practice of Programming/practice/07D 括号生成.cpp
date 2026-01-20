#include<iostream>
#include<cstring>
#include<algorithm>
#include<map>
#include<queue>
#include<iomanip>
#include<stack>

using namespace std;

int n;
bool check(string s) {
	stack<char>st;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '(')
			st.push('(');
		else {
			if (st.empty())
				return false;
			else
				st.pop();
		}
	}
	if (st.empty())
		return true;
	else
		return false;
}
int main() {
	cin >> n;
	string s;
	for (int i = 1; i <= n; i++)
		s.push_back('(');
	for (int i = 1; i <= n; i++)
		s.push_back(')');
	do {
		if (check(s))
			cout << s << endl;
	} while (next_permutation(s.begin(), s.end()));
	return 0;
}
