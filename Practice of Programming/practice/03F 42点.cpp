#include<iostream>
#include<cstring>
#include<algorithm>
#include<cmath>

using namespace std;

int n;
int num[10];
bool flag;

void dfs(int a[], int n) {
	if (flag)
		return;
	if (n == 1) {
		if (a[0] == 42)
			flag = true;
		return;
	}
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			int cnt = 0;
			int b[10] = { 0 };
			for (int k = 0; k < n; k++) {
				if (k == i || k == j)
					continue;
				b[cnt] = a[k];
				cnt++;
			}
			b[cnt] = a[i] + a[j];
			dfs(b, n - 1);
			b[cnt] = a[i] - a[j];
			dfs(b, n - 1);
			b[cnt] = a[j] - a[i];
			dfs(b, n - 1);
			b[cnt] = a[i] * a[j];
			dfs(b, n - 1);
			if (a[j] != 0) {
				b[cnt] = a[i] / a[j];
				dfs(b, n - 1);
			}
			if (a[i] != 0) {
				b[cnt] = a[j] / a[i];
				dfs(b, n - 1);
			}
		}
	}
}

int main()
{
	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> num[i];
	flag = false;
	dfs(num, n);
	if (flag)
		cout << "YES" << endl;
	else
		cout << "NO" << endl;
	return 0;
}
