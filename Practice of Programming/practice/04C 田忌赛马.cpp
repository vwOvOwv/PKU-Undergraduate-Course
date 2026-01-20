//ps.用dfs复写
#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<climits>
#include<vector>
using namespace std;

int n, x;
string sunbin;
string tianji;

int main()
{
	cin >> n >> x;
	for (int i = 0; i < n; i++) {
		int s;
		cin >> s;
		sunbin.push_back(s + '0');
	}
	for (int i = 1; i <= n; i++) {
		tianji.push_back(i + '0');
	}

	do {
		int cnt = 0;
		for (int i = 0; i < n; i++) {
			if (tianji[i] - sunbin[i] >= x)
				cnt++;
			else
				cnt--;
		}
		if (cnt > 0) {
			for (int i = 0; i < n; i++) {
				cout << tianji[i];
				if (i != n - 1)
					cout << ' ';
			}
			cout << endl;
		}
	} while (next_permutation(tianji.begin(), tianji.end()));
	return 0;
}
