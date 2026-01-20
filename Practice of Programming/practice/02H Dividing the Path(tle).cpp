#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>

using namespace std;

int f[1000010];//f[i]表示覆盖[0,i]的最少洒水头数量
/*
i满足以下条件：
1.i是偶数(洒水半径是整数且不能超出范围，i是奇数时无解)
2.i位置不能有奶牛，i?(S,E)(可以等于S,E)(i是两个洒水头交界处，必然有两个洒水头洒到这里)
3.i>=2*A
4.当i>2*B时，存在j∈[i-2*B,i-2*A]且j满足上述三个条件，使得f[i]=f[j]+1
  (喷到i处的洒水头最左端可以喷到的范围是[i-2*B,i-2*A])
5.f[i]=1:2*A<=i<=2*B
*/
int n, l, a, b;
int s,e;
int flag[1000010];
int main()
{
	cin >> n >> l >> a >> b;
	for (int i = 1; i <= n; i++) {
		cin >> s >> e;
		for (int i = s + 1; i < e; i++)
			flag[i] = 1;
	}
	if (l % 2 || l < 2 * a || flag[l]) {
		cout << "-1" << endl;
		return 0;
	}
	for (int i = 1; i <= l; i++) {
		if (i % 2||i<2*a||flag[i])
			f[i] = 1 << 30;
		else if (i >= 2 * a && i <= 2 * b)
			f[i] = 1;
		else {
			int mini = 1 << 30;
			for (int j = i - 2 * b; j <= i - 2 * a; j++) {
				mini = min(mini, f[j]);
				f[i] = mini + 1;
			}
		}
	}
	/*复杂度为O(1000000*1000),tle
	找数据里的最大值与最小值可以使用优先级队列优化(priority_queue)
	*/
	cout << f[l] << endl;
	return 0;
}
