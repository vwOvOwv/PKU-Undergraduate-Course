#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>

using namespace std;
//本质上就是求最长非降子序列长度，然后作差
int t;
string s;
int f[1010];//f[i]表示以s[i]为终点的最长非降子序列长度
int main()
{
	cin >> t;
	while (t--) {
		cin >> s;
		memset(f, 0, sizeof(f));
		for (int i = 0; i < s.length(); i++)
			f[i] = 1;
		for (int i = 1; i < s.length(); i++) {
			for (int j = 0; j < i; j++) {
				if (s[i] >= s[j])
					f[i] = max(f[i], f[j] + 1);
			}
		}
		cout << s.length() - *max_element(f,f+s.length()) << endl;
	}
	return 0;
}
