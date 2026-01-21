#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>

using namespace std;
int n, a;
int f(int a, int mini) {
	if (a == mini)
		return 1;
	int ans = 1;
	for (int i = mini; i*i <= a; i++) {
		if (a % i == 0) {
			ans += f(a / i, i);
		}
	}
	return ans;
}
int main()
{
	cin >> n;
	while (n--) {
		cin >> a;
		cout << f(a, 2) << endl;
	}
	return 0;
}
