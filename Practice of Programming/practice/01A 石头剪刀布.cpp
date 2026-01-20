#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>
using namespace std;
int a[110], b[110];
int main()
{
	int n, na, nb;
	cin >> n >> na >> nb;
	for (int i = 0; i < na; i++)
		cin >> a[i];
	for (int j = 0; j < nb; j++)
		cin >> b[j];
	int cnta = 0, cntb = 0;
	for (int i = 0; i < n; i++) {
		int x = i % na;
		int y = i % nb;
		if ((a[x] == 0 && b[y] == 2) || (a[x] == 2 && b[y] == 5) || (a[x] == 5 && b[y] == 0))
			cnta++;
		if ((a[x] == 0 && b[y] == 5) || (a[x] == 2 && b[y] == 0) || (a[x] == 5 && b[y] == 2))
			cntb++;
	}
	if (cnta > cntb)
		cout << 'A';
	else if (cnta < cntb)
		cout << 'B';
	else
		cout << "draw";
	return 0;
}

