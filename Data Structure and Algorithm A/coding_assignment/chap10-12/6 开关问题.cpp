#include <iostream> 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cmath>
#include <algorithm>
#define ll long long
using namespace std;
const int inf = 0x3fffffff;
const double eps = 1e-6;
const int N = 33;
int res, r;
int gauss(int a[][N], bool l[], int ans[], int &n){
    res = 0, r = 1;
    for (int i = 1; i <= n; i++){
        l[i] = false;
    }
    for (int i = 1; i <= n; i++){
        // 交换两行， 使得a[r][i]不为零
        for (int j = r; j <= n; j++){
            if (a[j][i]){
                for (int k = i; k <= n+1; k++){
                    swap(a[j][k], a[r][k]);
                }
                break;
            }
        }
        // 如果a[r][i]仍然是0 则这个方程是一个无用的方程 
        if (!a[r][i]){
            res++;
            continue;
        }
        for (int j = 1; j <= n; j++){
            if (j != r && a[j][i]){
                for (int k = i; k <= n+1; k++){
                    a[j][k] ^= a[r][k];
                }
            }
        }
        l[i] = true;
        r++;
    }
    for (int i = r; i <= n; i++){
        if (a[i][n+1] != 0){
            return 0;
        }
    }
    return 1;
}
int st[N], en[N];
bool l[N];
int ans[N];
int a[N][N];

int main(){
    int i, j, k, t;
    cin >> t;
    int n;
    while(t--){
        cin >> n;
        memset(a, 0, sizeof(a));
        for (i = 1; i <= n; i++){
            cin >> st[i];
        }
        for (i = 1; i <= n; i++){
            cin >> en[i];
        }
        while(cin >> i >> j){
            if (i == 0 && j == 0){
                break;
            }
            // a[j][i]表示改变第i个开关的状态第j个开关的状态也会改变
            a[j][i] = 1;
        }
        for (i = 1; i <= n; i++){
            a[i][i] = 1;
            a[i][n+1] = st[i]^en[i];
        }
        if (gauss(a, l, ans, n)){
            cout << (1 << res) << endl;
        }else{
            cout << "Oh,it's impossible~!!" << endl;
        }
    }
    return 0;
}