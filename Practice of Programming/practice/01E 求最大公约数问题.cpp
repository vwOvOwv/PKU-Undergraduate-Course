#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>

using namespace std;
int a, b;
int gcd(int a, int b) {
	return b ? gcd(b, a % b) : a;
}
int main()
{
	cin >> a >> b;
	cout << gcd(a, b);
	return 0;
}
