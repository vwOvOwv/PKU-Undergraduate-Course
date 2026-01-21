#include<iostream>
#include<cstring>
#include<algorithm>
#include<cmath>
using namespace std;
int p, q, r;
int trans(int a, int b) {
	int ans = 0;
	int cnt = 0;
	while (a != 0) {
		int t = a % 10;
		a /= 10;
		if (t >= b)
			return -1;
		ans += t * pow(b, cnt);
		cnt++;
	}
	return ans;
}
bool judge(int b) {
	if (trans(p, b) * trans(q, b) == trans(r, b))
		return true;
	else
		return false;
}
int main()
{

	cin >> p >> q >> r;
	int B;
	for (B = 2; B <= 16; B++) {
		if (judge(B)) {
			cout << B << endl;
			break;
		}
	}
	if (B == 17)
		cout << 0 << endl;
	return 0;
}
