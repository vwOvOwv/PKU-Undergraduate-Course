#include<iostream>
#include<cstring>
#include<algorithm>
#include<map>
#include<queue>
#include<iomanip>

using namespace std;

#define mem(a) memset(a,0,sizeof(a))
int t, n, k;
int pos[110], pro[110];
int f[110];
int main() {
	cin >> t;
	while (t--) {
		cin >> n >> k;
		mem(pos), mem(pro), mem(f);
		for (int i = 1; i <= n; i++)
			cin >> pos[i];
		for (int i = 1; i <= n; i++)
			cin >> pro[i];
		for (int i = 1; i <= n; i++)
			f[i] = pro[i];
		for (int i = 2; i <= n; i++) {
			for (int j = 1; j < i; j++) {
				if (pos[i] - pos[j] > k)
					f[i] = max(f[i], f[j] + pro[i]);
			}
		}
		cout << *max_element(f + 1, f + n + 1) << endl;
	}
	return 0;
}
