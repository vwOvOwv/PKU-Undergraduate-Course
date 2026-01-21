#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>

using namespace std;

int t, m;
int beijing[110], nanjing[110];
int f[110][2];

int main() {
	cin >> t >> m;
	for (int i = 1; i <= t; i++)
		cin >> beijing[i] >> nanjing[i];
	f[1][0] = beijing[1], f[1][1] = nanjing[1];
	for (int i = 2; i <= t; i++) {
		f[i][0] = max(f[i - 1][0] + beijing[i], f[i - 1][1] + beijing[i] - m);
		f[i][1] = max(f[i - 1][0] + nanjing[i] - m, f[i - 1][1] + nanjing[i]);
	}
	cout << max(f[t][0], f[t][1]) << endl;
	return 0;
}
