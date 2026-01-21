#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>
using namespace std;
int n;
int mini = INT_MAX;
int main()
{
	cin >> n;
	for (int a = 1; a <= n; a++) {
		for (int b = 1; b <= n / a; b++) {
			if (n % (a * b) == 0) {
				int c = n / (a * b);
				mini = min(mini, 2 * (a * b + a * c + b * c));
			}
		}
	}
	cout << mini;
	return 0;
}
