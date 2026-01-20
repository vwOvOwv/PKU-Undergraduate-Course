#include <iostream>

using namespace std;

int n, m;

int josephus(int n, int m){
    int res = 1;
    for(int i = 1; i <= n; i++){    // i: current number of people
        res = (res + m) % i; 
        // cout << res + 1 << endl;
    }
    return res + 1;
}

int main(){
    while (cin >> n >> m){
        if (n == 0 && m == 0){
            break;
        }
        cout << josephus(n, m) << endl;
    }
    return 0;
}