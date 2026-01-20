#include<iostream>
#include<vector>
#include<cstring>
#include<algorithm>
#include<climits>
#include<cmath>
using namespace std;
int N, M;
int min_area = INT_MAX;//最优表面积
int area = 0;//正在搭建中的蛋糕的表面积
void dfs(int v, int n, int r, int h) {//用n层凑体积v，最底层半径不能超过r，高度不能超过h
    //还可以考虑还没搭的体积的上下界与仍缺的体积的关系进行可行性剪枝
    if (h < n || r  < n)//可行性剪枝
        return;
    if (n == 0) {
        if (v)
            return;
        else {
            min_area = min(min_area, area);
            return;
        }
    }
    if (v <= 0)
        return;
    for (int rr = r; rr >= n; rr--) {//第n层半径至少为n
        if (n == M)//最底层
            area = rr * rr;//投影
        for (int hh = h; hh >= n; hh--) {//第n层高度至少为n
            if (area + 2 * rr * hh < min_area) {//最优性剪枝
                area += 2 * rr * hh;
                dfs(v - rr * rr * hh, n - 1, rr - 1, hh - 1);
                area -= 2 * rr * hh;
            }
        }
    }
}
int main()
{
    cin >> N >> M;
    int max_r = sqrt(N);
    int max_h = N;
    dfs(N, M, max_r, max_h);
    if (min_area == INT_MAX)
        cout << 0;
    else
        cout << min_area;
    return 0;
}
