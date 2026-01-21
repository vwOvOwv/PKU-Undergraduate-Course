#include<iostream>
#include<cstring>
#include<algorithm>
#include<climits>
#include<vector>

using namespace std;
int n;
long long int f[40];//f[i]表示i列的覆盖方式总数
/*
i为奇数是无法完美覆盖，f[i]=0;
i为偶数时，f[0]=1,f[2]=3;
对任意i>=4,都可以将i分为左边的i-2,i-4,...,2,0和右边的2,4,...,i-2,i
注意为了避免重复，右边的部分必然是不可分割的，如4不能由两个2拼成...
归纳可以发现，对于右侧的拼法a[i],a[i]=i>2?2:3
故f[i]=3*f[i-2]+2*(f[i-4]+f[i-6]+...+f[2]+f[0])
f[i-2]=3*f[i-4]+2*(f[i-6]+f[i-8]+...f[2]+f[0])
->f[i]=4*f[i-2]-f[i-4],i>=4
*/
int main()
{
	f[0] = 1;
	f[2] = 3;
	for (int i = 4; i <= 30; i++)
		f[i] = 4 * f[i - 2] - f[i - 4];
	while (cin >> n) {
		if (n == -1)
			break;
		if (n % 2 == 1) {
			cout << '0' << endl;
			continue;
		}
		cout << f[n] << endl;
	}
	return 0;
}
