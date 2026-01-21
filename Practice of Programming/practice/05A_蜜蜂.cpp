#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>

using namespace std;

long long int f[60][60];
int n, a, b;

int main()
{
	for (int i = 1; i < 49; i++) {
		f[i][i] = 1;
		f[i][i + 1] = 1;
	}
	for (int i = 1; i < 50; i++) {
		for (int j = i+2; j < 50; j++) {
			f[i][j] = f[i][j - 1] + f[i][j - 2];
		}
	}
	cin >> n;
	while (n--) {
		cin >> a >> b;
		cout << f[a][b] << endl;
	}
	return 0;
}
