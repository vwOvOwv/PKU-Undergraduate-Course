//本题思路明显，从左到右和从右到左分别求以每个位置为终点的最长上升子序列（中间可以有更高的楼）
#include<algorithm>
#include<vector>
#include<iostream>
#include<cstring>
using namespace std;
int main()
{
	int dp1[110] = { 0 }, dp2[110] = { 0 }, h[110] = { 0 };
	int t, n;
	cin >> t;
	while (t--) {
		memset(dp1, 0, sizeof(dp1));
		memset(dp2, 0, sizeof(dp2));
		memset(h, 0, sizeof(h));
		cin >> n;
		for (int i = 1; i <= n; i++)
			cin >> h[i];
		dp1[1] = 1;
		dp2[n] = 1;
		for (int i = 2; i <= n; i++) {
			dp1[i] = 1;
			for (int j = 1; j < i; j++) {
				if (h[i] > h[j]) {
					dp1[i] = max(dp1[i], dp1[j] + 1);
				}
			}
		}
		for (int i = n - 1; i >= 1; i--) {
			dp2[i] = 1;
			for (int j = n; j > i; j--) {
				if (h[i] > h[j]) {
					dp2[i] = max(dp2[i], dp2[j] + 1);
				}
			}
		}
		cout << max(*max_element(dp1 + 1, dp1 + n + 1), *max_element(dp2 + 1, dp2 + n + 1)) << endl;
	}
	return 0;
}
