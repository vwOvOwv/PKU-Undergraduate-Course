//经典的二分查找
#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<climits>
#include<vector>
#include<cmath>
using namespace std;

int n, m;
int a[1010];
int main()
{
	cin >> n >> m;
	int mini = INT_MAX;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mini = min(a[i], mini);
	}
	int L = 0, R = INT_MAX;
	int ans;
	while (L <= R) {
		int mid = L + (R - L) / 2;
		int cnt = 0;
		for (int i = 1; i <= n; i++) {
			cnt += ceil((a[i] * 1.0) / mid);
		}
		if (cnt > m) {
			L = mid + 1;
		}
		else {
			ans = mid;
			R = mid - 1;
		}
	}
	cout << ans;
	return 0;
}
