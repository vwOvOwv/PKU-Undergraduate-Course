#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
int n;
int f[1010][1010];
string s1, s2;
int mymin(int a, int b, int c) {
	int ans = a;
	ans = min(ans, b);
	ans = min(ans, c);
	return ans;
}
int main()
{
	cin >> n;
	while (n--) {
		memset(f, 0, sizeof(f));
		s1.clear();
		s2.clear();
		string tmp1, tmp2;
		cin >> tmp1 >> tmp2;
		s1.push_back(' ');
		s2.push_back(' ');
		s1 += tmp1;
		s2 += tmp2;
		int l1 = s1.length(), l2 = s2.length();
		for (int i = 1; i <= l1; i++)
			f[i][0] = i;
		for (int j = 1; j <= l2; j++)
			f[0][j] = j;
		for (int i = 1; i <= l1; i++) {
			for (int j = 1; j <= l2; j++) {
				if (s1[i] == s2[j])
					f[i][j] = f[i - 1][j - 1];
				else
					f[i][j] = mymin(f[i - 1][j - 1], f[i - 1][j], f[i][j - 1]) + 1;
			}
		}
		cout << f[l1][l2] << endl;
	}
	return 0;
}
