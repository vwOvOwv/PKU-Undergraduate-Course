#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>
using namespace std;

int a[10][10];
int maxi[10];
int id[10];

int main()
{
	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 5; j++) {
			cin >> a[i][j];
			maxi[i] = max(maxi[i], a[i][j]);
			if (maxi[i] == a[i][j])
				id[i] = j;
		}
	}
	for (int i = 1; i <= 4; i++) {
		int j = id[i], k;
		for (k = 1; k <= 4; k++) {
			if (maxi[i] > a[k][j])
				break;
		}
		if (k == 5) {
			cout << i << ' ' << j << ' ' << maxi[i] << endl;
			return 0;
		}
	}
	cout << "not found" << endl;
	return 0;
}
