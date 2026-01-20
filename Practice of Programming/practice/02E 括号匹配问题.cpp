#include<iostream>
#include<cstring>
#include<algorithm>
#include<stack>
#include<iomanip>
#include<string>
using namespace std;


stack<int>st;

int main()
{
	string s;
	while (getline(cin, s)) {
		cout << s << endl;
		while (!st.empty())
			st.pop();
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '(')
				st.push(i);
			if (s[i] == ')')
				if (!st.empty()) {
					s[st.top()] = ' ';
					st.pop();
					s[i] = ' ';
				}
		}
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '(')
				s[i] = '$';
			else if (s[i] == ')')
				s[i] = '?';
			else
				s[i] = ' ';
		}
		cout << s << endl;
	}
	return 0;
}
