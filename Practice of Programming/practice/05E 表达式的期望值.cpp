#include<iostream>
#include<cstring>
#include<algorithm>
#include<map>
#include<queue>
#include<iomanip>
#include<stack>

using namespace std;

//E[X]=E[sum{2^i*Xi,0<=i<=20}=sum{2^i*E[Xi],0<=i<=20}
/*
位运算每一位都是相互独立的
所以可以利用位运算计算每一位的期望值
进而得到总的期望值
而每一位均是0-1分布，所以期望值即为该项为1的概率
*/

double calculate(double left, char op, double right) {
	switch (op)
	{
	case '^':
		return right ? 1 - left : left;//右侧数是1则运算后为1的期望是1-E(左侧数为0的期望)
	case '|':
		return right ? 1 : left;
	case '&':
		return right ? left : 0;
	default:
		return 0;
	}
}
int num[210];
char ops[210];
double prob[210];

int main() {
	int t = 0, n;
	while (cin >> n) {
		n++;
		t++;
		double result = 0;
		int digit = 0;
		for (int i = 0; i < n; i++)
			cin >> num[i];
		for (int i = 1; i < n; i++)
			cin >> ops[i];
		for (int i = 1; i < n; i++)
			cin >> prob[i];
		for (int i = 0; i < 21; i++) {//num[i]<2^20
			double E = (double)((num[0] >> i) & 1);//初始化，首个数的第i位即为其期望,因为首个数不可能消失
			for (int j = 1; j < n; j++)
				E = prob[j] * E + (1 - prob[j]) * calculate(E, ops[j], (num[j] >> i) & 1);
			//若第j个运算符消失，则第i位的期望和前一位的期望相同(E)，若不消失则计算期望
			result += E * (double)(1 << digit);//转换成十进制数
			digit++;
		}
		cout << "Case " << t << ':' << endl;
		cout << fixed << setprecision(6) << result << endl;
	}
	return 0;
}
