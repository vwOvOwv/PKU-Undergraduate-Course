#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>

using namespace std;

int n, m;
int f[110][20];//f[i][j]表示i层楼j个鸡蛋在最坏情况下所需要的最少扔鸡蛋次数

int main()
{
	for (int i = 1; i <= 100; i++) {
		for (int j = 1; j <= 10; j++) {
			f[i][j] = 1 << 30;//重要！！！下面是求最小，不能初始化为0！！！
		}
	}
	for (int i = 1; i <= 100; i++)
		f[i][1] = i;//i层1个鸡蛋最坏是硬度为i
	for (int j = 1; j <= 10; j++)
		f[1][j] = 1;//1层任意多个鸡蛋都要扔一次看硬度是1还是0
	for (int i = 2; i <= 100; i++) {
		for (int j = 2; j <= 10; j++) {
			for (int k = 1; k <= i; k++) {
				f[i][j] = min(f[i][j], max(f[i - k][j], f[k - 1][j - 1]) + 1);
				/*
				min表示最优策略，max表示最坏情况
				从1层到i层一层一层扔：
				如果碎了则鸡蛋硬度在[0,k-1]之间，只需要在前k-1层扔j-1个鸡蛋就可以
				如果没碎说明鸡蛋硬度在[k,i]之间，应在[k+1,i]层扔，共i-(k+1)+1=i-k，等价于在前[i-k]层扔j个鸡蛋
				注意+1
				本题关键点在于最坏情况的鸡蛋硬度是动态变化的，即每个状态都有鸡蛋碎或不碎两种情况，不碎时有上述等价条件
				*/
			}
		}
	}
	while (cin >> n >> m) {
		cout << f[n][m] << endl;
	}
}
