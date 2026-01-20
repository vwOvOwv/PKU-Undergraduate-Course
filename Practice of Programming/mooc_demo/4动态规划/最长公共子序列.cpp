/*
设maxlen[i][j]表示：
s1的前i个字符形成的子串与s2的前j个字符形成的子串的最长公共子序列的长度
（i,j应从0开始）
显然，maxlen[0][j]=0,maxlen[i][0]=0;
状态转移方程：
if(s1[i-1]==s2[j-1])
	maxlen[i][j]=maxlen[i-1][j-1]+1;
else
	maxlen[i][j]=max(maxlen[i-1][j],maxlen[i][j-1]);
求maxlen[length1][length2]即可
*/
#include<iostream>
#include<cstring>
using namespace std;
char sz1[210], sz2[210];
int maxlen[210][210];
int main()
{
	while (cin >> sz1 >> sz2) {
		int length1 = strlen(sz1);
		int length2 = strlen(sz2);
		for (int i = 0; i <= length1; i++)
			maxlen[i][0] = 0;
		for (int j = 0; j <= length2; j++)
			maxlen[0][j] = 0;
		for (int i = 1; i <= length1; i++)
			for (int j = 1; j <= length2; j++) {
				if (sz1[i - 1] == sz2[j - 1])
					maxlen[i][j] = maxlen[i - 1][j - 1] + 1;
				else
					maxlen[i][j] = max(maxlen[i][j - 1], maxlen[i - 1][j]);
			}
		cout << maxlen[length1][length2] << endl;
	}
	return 0;
}

