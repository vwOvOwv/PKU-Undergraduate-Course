#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>

using namespace std;

int f[1000010];//f[i]表示覆盖[0,i]的最少洒水头数量
/*
i满足以下条件：
1.i是偶数(洒水半径是整数且不能超出范围，i是奇数时无解)
2.i位置不能有奶牛，i?(S,E)(可以等于S,E)(i是两个洒水头交界处，必然有两个洒水头洒到这里)
3.i>=2*A
4.当i>2*B时，存在j∈[i-2*B,i-2*A]且j满足上述三个条件，使得f[i]=f[j]+1
  (喷到i处的洒水头最左端可以喷到的范围是[i-2*B,i-2*A])
5.f[i]=1:2*A<=i<=2*B
*/
int n, l, a, b;
int s,e;
int flag[1000010];
struct Fx
{
	int x, f;
	bool operator<(const Fx& a)const {
		return f > a.f;//f越小优先级越高
	}
	Fx(int xx, int ff) :x(xx), f(ff) {}
	Fx() {}
};
priority_queue<Fx>qFx;
int main()
{
	cin >> n >> l >> a >> b;
	for (int i = 1; i <= n; i++) {
		cin >> s >> e;
		++flag[s + 1];
		--flag[e];
	}
	int inCow = 0;//当前点位于多少头奶牛的活动范围之内
	for (int i = 0; i <= l; i++) {//算出每个点是否有奶牛
		f[i] = 1 << 30;
		inCow += flag[i];
		flag[i] = inCow > 0;
	}
	for (int i = 2 * a; i <= 2 * b; i += 2) {
		if (!flag[i]) {
			f[i] = 1;
			if (i <= 2 * b + 2 - 2 * a)
				qFx.push(Fx(i, 1));
		}
	}//递推初始化
	for (int i = 2 * b + 2; i <= l; i += 2) {//O(nlog(n))
		if (!flag[i]) {//不在奶牛区域中
			Fx fx;
			while (!qFx.empty()) {
				fx = qFx.top();
				if (fx.x < i - 2 * b)//对递推无用
					qFx.pop();
				else
					break;
			}
			if (!qFx.empty())//找到了合适的fx
				f[i] = fx.f + 1;
		}
		if (f[i - 2 * a + 2] != 1 << 30)//为下一步递推做准备
			qFx.push(Fx(i - 2 * a + 2, f[i - 2 * a + 2]));
	}
	if (f[l] == 1 << 30)
		cout << -1 << endl;
	else
		cout << f[l] << endl;
	return 0;
}
