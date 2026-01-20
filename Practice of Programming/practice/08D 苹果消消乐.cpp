#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>

using namespace std;

int t, n, m;
int a[110];

int main() {
	cin >> t;
	while (t--) {
		cin >> n >> m;
		for (int i = 1; i <= n; i++)
			cin >> a[i];
		a[n + 1] = 101;
		int num = min(n, m);
		int ans = 0;
		for (int i = 0; i <= num; i++) {//枚举操作次数
			for (int j = 1; j + i - 1 <= n; j++) {//枚举施法范围
				ans = max(ans, a[j + i] - a[j - 1] - 1);//后一个香蕉位置-前一个香蕉位置-1
			}
		}
		cout << ans << endl;
	}
	return 0;
}
