#include<iostream>
#include<cstring>
using namespace std;
int a[30], n;
int ways[50][30];//ways[i][j]表示从前j种物品里凑出体积i的方法数
int main()
{
	cin >> n;
	memset(ways, 0, sizeof(ways));
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		ways[0][i] = 1;
	}
	ways[0][0] = 1;
	for (int w = 1; w <= 40; w++) {
		for (int k = 1; k <= n; k++) {
			ways[w][k] = ways[w][k - 1];
			if (w - a[k] >= 0)
				ways[w][k] += ways[w - a[k]][k - 1];
		}
	}
	cout << ways[40][n];
	return 0;
}
