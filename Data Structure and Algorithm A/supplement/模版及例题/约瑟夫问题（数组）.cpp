#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
//原始的实现方法
int n, m;
vector<int> rec;
int main()
{
    while (cin >> n >> m && n)
    {
        rec.clear();
        for (int i = 0; i <= n; i++)
        {
            rec.push_back(i);
        }
        int now = n, cnt = 0, index = 0;
        while (now != 1)
        {
            cnt++;
            index++;
            if (index == n + 1)//特判范围
                index = 0;
            while (rec[index] == 0)
            {
                index++;
                if (index == n + 1)
                    index = 0;
            }
            // cout<<index<<endl;
            if (cnt == m)
            {
                now--;
                rec[index] = 0;
                cnt = 0;
            }
        }
        unique(rec.begin(), rec.end());
        cout << rec[1] << endl;
    }
    return 0;
}