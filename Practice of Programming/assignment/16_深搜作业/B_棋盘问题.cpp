#include<iostream>
#include<cstring>
using namespace std;
int n, k, cnt;
char a[15][15];
void dfs(int i) {
	if (k == 0) {
		cnt++;
		return;
	}
	if (i == n + 1)//注意这里应当让i多走一行
		return;
	for (int j = 1; j <= n; j++) {//遍历每个位置
		if (a[i][j] == '#') {
			int r;
			for (r = 1; r <= i; r++)
				if (a[r][j] == 'x')
					break;
			if (r == i + 1) {//可以放子
				a[i][j] = 'x';
				k--;
				dfs(i + 1);
				k++;
				a[i][j] = '#';//回溯
			}
		}
	}
	dfs(i + 1);//注意还可以不在这一行放子！！
}
int main()
{
	while (cin >> n >> k) {
		if (k == -1 && n == -1)
			break;
		memset(a, 0, sizeof(a));
		cnt = 0;
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				cin >> a[i][j];
			}
		}
		dfs(1);
		cout << cnt << endl;
	}
	return 0;
}
