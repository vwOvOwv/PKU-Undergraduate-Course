#include<iostream>
#include<cstring>
#include<algorithm>
#include<stack>
#include<iomanip>
using namespace std;
stack<string>st;
stack<double>ans;
int main()
{
	string s;
	while (cin >> s) {
		st.push(s);
	}
	while (!st.empty()) {
		string t = st.top();
		st.pop();
		if (t[0] >= '0' && t[0] <= '9')
			ans.push(atof(t.c_str()));
		else {
			double tmp;
			switch (t[0])
			{
			case '+':
				tmp = ans.top();
				ans.pop();
				tmp += ans.top();
				ans.pop();
				ans.push(tmp);
				break;
			case '-':
				tmp = ans.top();
				ans.pop();
				tmp -= ans.top();
				ans.pop();
				ans.push(tmp);
				break;
			case '*':
				tmp = ans.top();
				ans.pop();
				tmp *= ans.top();
				ans.pop();
				ans.push(tmp);
				break;
			case '/':
				tmp = ans.top();
				ans.pop();
				tmp /= ans.top();
				ans.pop();
				ans.push(tmp);
				break;
			}
		}
	}
	cout << fixed << setprecision(6) << ans.top() << endl;
	return 0;
}
