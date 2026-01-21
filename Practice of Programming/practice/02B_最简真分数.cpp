#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
int n;
int a[610];
int gcd(int a, int b) {
	return b ? gcd(b, a % b) : a;
}
int main()
{
	cin >> n;
	for (int i = 1; i <= n; i++)
		cin >> a[i];
	int cnt = 0;
	for (int zi = 1; zi <= n; zi++) {
		for (int mu = 1; mu <= n; mu++) {
			if (zi == mu)
				continue;
			if (gcd(a[zi], a[mu]) == 1)
				cnt++;
		}
	}
	cout << cnt / 2 << endl;//"组合的个数"
	return 0;
}
