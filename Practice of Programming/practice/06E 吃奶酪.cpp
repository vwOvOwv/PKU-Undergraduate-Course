#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>
#include<queue>

using namespace std;

int t;
int n;
long long int a[100010];
long long int f[100010];
int main()
{
	cin >> t;
	while (t--) {
		memset(f, 0, sizeof(f));
		memset(a, 0, sizeof(f));
		cin >> n;
		for (int i = 1; i <= n; i++)
			cin >> a[i];
		int k;
		for (k = 1; k <= n; k++) {//注意要找第一个正的美味度
			if (a[k] > 0) {
				f[k] = a[k];
				break;
			}
		}
		for (int i = k + 1; i <= n; i++) {
			f[i] = max(f[i - 1], f[i - 2] + a[i]);
		}
		cout << f[n] << endl;
	}

	return 0;
}
