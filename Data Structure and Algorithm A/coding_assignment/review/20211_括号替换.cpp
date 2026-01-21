#include<iostream>
#include<vector>
#include<stack>
#include<string>

using namespace std;

int main() {
	string s;
	cin >> s;
	stack<char>st;
	int n = s.size();
	int cnt = 0;
	for (int i = 0; i < n; i++) {
		if (s[i] == '[' || s[i] == '<' || s[i] == '{' || s[i] == '(')
			st.push(s[i]);
		else {
			if (st.empty()) {
				cout << "Impossible" << endl;
				return 0;
			}
			else {
				char tmp = st.top();
				st.pop();
				if ((s[i] == ']' && tmp != '[') || (s[i] == '>' && tmp != '<') || (s[i] == '}' && tmp != '{') || (s[i] == ')' && tmp != '('))
					cnt++;
			}
		}
	}
	if (st.empty())
		cout << cnt << endl;
	else
		cout << "Impossible" << endl;
}