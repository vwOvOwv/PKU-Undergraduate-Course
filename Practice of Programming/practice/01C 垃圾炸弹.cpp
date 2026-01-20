#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
int d, n;
int memx[25], memy[25];
int trash[1030][1030];
int flag[1030][1030];
int maxi = 0;
int cntp = 0;
int cnt = 0;
int num;
int main()
{
	cin >> d >> n;
	for (int i = 0; i < n; i++) {
		cin >> memx[i] >> memy[i] >> num;
		trash[memx[i]][memy[i]] = num;
	}
	for (int i = 0; i < n; i++) {
		int x = memx[i], y = memy[i];
		for (int j = x - d; j <= x + d; j++) {
			for (int k = y - d; k <= y + d; k++) {
				if (j >= 0 && j <= 1024 && k >= 0 && k <= 1024&&flag[j][k]==0) {
					flag[j][k] = 1;
					cnt = 0;
					for (int jj = j - d; jj <= j + d; jj++) {
						for (int kk = k - d; kk <= k + d; kk++) {
							if (jj >= 0 && jj <= 1024 && kk >= 0 && kk <= 1024 && trash[jj][kk] != 0) {
								cnt += trash[jj][kk];
							}
						}
					}
					if (cnt > maxi) {
						maxi = cnt;
						cntp = 1;
					}
					else if (cnt == maxi)
						cntp++;
				}
			}
		}
	}
	cout << cntp << ' ' << maxi;
	return 0;
}
