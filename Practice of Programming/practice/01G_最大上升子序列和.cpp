#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
int n;
int f[1010];
int a[1010];
int main()
{
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		f[i] = a[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= i; j++) {
			if (a[i] > a[j])
				f[i] = max(f[i], f[j] + a[i]);
		}
	}
	cout << *max_element(f + 1, f + n + 1);
	return 0;
}
