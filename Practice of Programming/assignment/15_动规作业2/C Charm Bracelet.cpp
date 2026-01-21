#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
const int N = 12900;
int dp[N];
int n, m;
int v, w;
int main()
{
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> v >> w;
        for (int j = m; j >= v; j--)
            dp[j] = max(dp[j], dp[j - v] + w);
    }
    cout << dp[m];
    return 0;
}
