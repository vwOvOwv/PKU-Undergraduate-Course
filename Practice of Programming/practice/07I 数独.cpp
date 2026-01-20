#include<iostream>
#include<vector>
#include<iomanip>
#include<queue>
#include<algorithm>
#include<cstring>
#include<map>

using namespace std;

#define mem(a) memset(a,0,sizeof(a))
int t;
int block[10][10];
bool rec_r[10][10], rec_c[10][10];
bool flag;
void dfs(int n,int m) {
	if (flag)
		return;
	if (m == 10) {
		dfs(n + 1, 1);
		return;
	}
	if (n == 10) {
		for (int i = 1; i <= 9; i++) {
			for (int j = 1; j <= 9; j++) {
				cout << block[i][j];
			}
			cout << endl;
		}
		flag = true;
		return;
	}
	int cnt_x = n % 3 ? n / 3 + 1 : n / 3;
	int cnt_y = m % 3 ? m / 3 + 1 : m / 3;
	int cnt[10] = { 0 };
	for (int i = 3 * cnt_x - 2; i <= 3 * cnt_x; i++) {
		for (int j = 3 * cnt_y - 2; j <= 3 * cnt_y; j++) {
			cnt[block[i][j]] = 1;
		}
	}
	if (!block[n][m]) {
		for (int num = 1; num <= 9; num++) {
			if (!rec_r[n][num] && !rec_c[m][num]) {
				if (!cnt[num]) {
					rec_r[n][num] = rec_c[m][num] = 1;
					block[n][m] = num;
					dfs(n, m + 1);
					block[n][m] = 0;
					rec_r[n][num] = rec_c[m][num] = 0;
				}
			}
		}
	}
	else
		dfs(n, m + 1);
}

int main() {
	cin >> t;
	while (t--) {
		mem(block), mem(rec_r), mem(rec_c);
		flag = false;
		for (int i = 1; i <= 9; i++) {
			for (int j = 1; j <= 9; j++) {
				char c;
				cin >> c;
				block[i][j] = c - '0';
				if (c - '0' > 0) {
					rec_r[i][c - '0'] = 1;
					rec_c[j][c - '0'] = 1;
				}
			}
		}
		dfs(1, 1);
	}
}
