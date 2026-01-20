#include<iostream>
#include<cstring>
#include<algorithm>
#include<map>
#include<queue>

using namespace std;
//不能用dp就考虑dfs，注意数据规模
int sp;
int w[20], s[20];
int cnt10, cnt5;
int money, maxi = 1 << 31;
void dfs(int k) {
	if (k == 16) {
		if (cnt10 <= cnt5)
			maxi = max(money, maxi);
		return;
	}
	if (sp >= s[k]) {
		sp -= s[k];
		money += w[k];
		if (k <= 10)
			cnt10++;
		if (k > 10)
			cnt5++;
		dfs(k + 1);
		if (k <= 10)
			cnt10--;
		if (k > 10)
			cnt5--;
		money -= w[k];
		sp += s[k];
	}
	dfs(k + 1);
}

int main() {
	cin >> sp;
	for (int i = 1; i <= 15; i++)
		cin >> w[i] >> s[i];
	dfs(1);
	cout << maxi;
}
