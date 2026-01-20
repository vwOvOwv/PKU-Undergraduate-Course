/*
设f[i][j]表示前i个数中添加j个加号，有j<i
f[i][j]=min{f[k][j-1]+num[k+1][i]} k=j,j+1,...,i-1;
确定i,j,k的范围是重要的
递推思路：
考虑最右边的等号出现的位置（有多种可能位置：k），从所有可能中选取最小的即为所要求的f[i][j]
答案是f[n][m]
边界条件：f[k][0]=num(1,k);
*/
#include<iostream>
#include<string>
#include<algorithm>
#include<cstring>
using namespace std;
string f[60][60];
string num[60][60];
string _plus(string a, string b) {//高精度加法
	string ans(60, ' ');
	int tmp = 59;
	for (int i = a.length() - 1, j = b.length() - 1; i >= 0 || j >= 0; i--, j--) {
		if (i < 0 && j >= 0) {
			ans[tmp] = b[j];
		}
		else if (j < 0 && i >= 0) {
			ans[tmp] = a[i];
		}
		else {
			ans[tmp] = a[i] + b[j] - '0';
		}
		tmp--;
	}
	for (int i = 59; i > tmp; i--) {
		if (ans[i] >'9') {
			ans[i] -= 10;
			if (i - 1 == tmp) {
				ans[tmp] = '1';
			}
			else {
				ans[i - 1] += 1;
			}
		}
	}
	int i;
	for (i = 0; ans[i] == ' '; i++);
	string res = ans.substr(i);
	return res;
}
bool _less(string a, string b) {//比较高精度整数大小
	if (a.length() == b.length())
		return a < b;
	else {
		if (a.length() < b.length())
			return true;
		else
			return false;
	}
}
int main()
{
	int m;
	string s;
	while (cin >> m >> s) {
		memset(f, 0, sizeof(0));
		memset(num, 0, sizeof(0));
		s.insert(0, "0");
		int n = s.length() - 1;
		//截取整数
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= i; j++) {
				if (i == j) {
					num[i][j] = s[i];
				}
				else {
					string t = s.substr(j, i - j + 1);
					num[i][j] = t;
					num[j][i] = t;
				}
			}
		}
		for (int i = 1; i <= n; i++)
			f[i][0] = num[1][i];//dp初始化：没有加号时有几个数字就是几位数
		for (int i = 2; i <= n; i++) {
			for (int j = 1; j < i; j++) {
				string ans;string mini(100, '9');
				for (int k = j; k < i; k++) {
					string res = _plus(f[k][j - 1], num[k + 1][i]);
					if (_less(res,mini)){
						mini = res;
						ans = mini;
					}
				}
				f[i][j] = ans;
			}
		}
		cout << f[n][m] << endl;
	}
	return 0;
}
