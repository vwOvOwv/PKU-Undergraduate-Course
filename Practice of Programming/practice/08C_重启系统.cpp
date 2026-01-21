#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>

using namespace std;

int n;
int a[1010];
int cnt, maxi;
int f[1010];
int mem1[1010], mem2[1010];

int main() {
	cin >> n;
	for (int i = 1; i <= n; i++)
		cin >> a[i];
	for (int i = 1; i <= n; i++)
		mem1[i] = mem2[i] = 1;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j < i; j++) {
			if (a[i] <= a[j]) {
				mem1[i] = max(mem1[i], mem1[j] + 1);
			}
		}
	}
	for (int i = n; i >= 1; i--)  {
		for (int j = i + 1 ; j <= n; j++) {
			if (a[j] <= a[i]) {
				mem2[i] = max(mem2[i], mem2[j] + 1);
			}
		}
	}
	for (int i = 1; i <= n; i++)
		f[i] = *max_element(mem1 + 1, mem1 + i + 1) + *max_element(mem2 + i + 1, mem2 + n + 1);
	cout << *max_element(f + 1, f + n + 1);
	return 0;
}
