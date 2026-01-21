#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<climits>
#include<map>
using namespace std;

int t;
int n;
int a[100010];
int f[100010];
int mmin[100010];
int mmax[100010];
int first[100010];
int second[100010];

void init() {
	memset(a, 0, sizeof(a));
	memset(f, 0, sizeof(f));
	memset(mmin, 0, sizeof(mmin));
	memset(mmax, 0, sizeof(mmax));
	memset(first, 0, sizeof(first));
	memset(second, 0, sizeof(second));
}
int main()
{
	cin >> t;
	while (t--) {
		cin >> n;
		init();
		for (int i = 1; i <= n; i++)
			cin >> a[i];
		mmin[1] = a[1];
		first[1] = 0;
		for (int i = 2; i <= n; i++) {
			mmin[i] = min(mmin[i - 1], a[i]);
			first[i] = max(first[i - 1], a[i] - mmin[i]);
		}
		mmax[n] = a[n];
		second[n] = 0;
		for (int i = n - 1; i >= 1; i--) {
			mmax[i] = max(mmax[i + 1], a[i]);
			second[i] = max(second[i + 1], mmax[i] - a[i]);
			f[i] = first[i] + second[i];
		}
		cout << *max_element(f + 1, f + n + 1) << endl;
	}
	return 0;
}
