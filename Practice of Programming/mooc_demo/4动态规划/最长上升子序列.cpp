//递推型动态规划
//子问题：求以ak为终点（包含ak）的最长上升子序列的长度
//ps.不满足无后效性的子问题无法写出递推方程
#include<iostream>
#include<algorithm>
using namespace std;
const int maxn = 1010;
int a[maxn];
int maxlen[maxn];
int main()
{
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		maxlen[i] = 1;
	}
	for (int i = 2; i <= n; i++)//每次求以第i个数为终点的最长上升子序列长度
		for (int j = 1; j < i; j++)//查看以第j个数为终点的最长上升子序列长度
			if (a[i] > a[j])
				maxlen[i] = max(maxlen[i], maxlen[j] + 1);//注意+1
	cout << *max_element(maxlen + 1, maxlen + n + 1);
	return 0;
}
