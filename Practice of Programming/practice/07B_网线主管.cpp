#include<iostream>
#include<cstring>
#include<algorithm>
#include<map>
#include<queue>
#include<iomanip>

using namespace std;

int n, k;
int length[10010];

int main() {
	int ans = 1 << 31;
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		double s;
		cin >> s;
		length[i] = s * 100;
	}
	int L = 1, R = 10000000;
	while (L <= R) {
		int mid = L + (R - L) / 2;
		int cnt = 0;
		for (int i = 1; i <= n; i++)
			cnt += length[i] / mid;
		if (cnt >= k) {
			ans = mid;
			L = mid + 1;
		}
		else
			R = mid - 1;
	}
	if (ans != 1 << 31)
		cout << fixed << setprecision(2) << (ans * 1.0) / 100 << endl;
	else
		cout << "0.00" << endl;
	return 0;
}
