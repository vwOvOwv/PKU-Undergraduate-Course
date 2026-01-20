#include<iostream>
#include<queue>
#include<algorithm>
#include<cmath>
#include<cstring>
using namespace std;

int cnt_prime(int n) {
	int cnt = 0;
	for (int i = 2; i <= sqrt(n); i++) {//减少循环次数避免超时
		if (n % i == 0) {
			int j = n / i;
			int p, flag = 0;
			for (p = 2; p <i; p++) {
				if (i % p == 0)
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				cnt++;
			}
			flag = 0;
			for (p = 2; p < j; p++) {
				if (j % p == 0)
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				cnt++;
			}
			flag = 0;
		}
	}
	return cnt;
}

class Compare1 {
public:
	bool operator()(int& a, int& b) {
		if (cnt_prime(a) < cnt_prime(b))
			return false;
		else if (cnt_prime(a) > cnt_prime(b))
			return true;
		else {
			if (a < b)
				return false;
			else
				return true;
		}
	}
};

class Compare2 {
public:
	bool operator()(int& a, int& b) {
		if (cnt_prime(a) > cnt_prime(b))
			return false;
		else if (cnt_prime(a) < cnt_prime(b))
			return true;
		else {
			if (a > b)
				return false;
			else
				return true;
		}
	}
};

int main()
{
	priority_queue<int, vector<int>, Compare1>pq1;
	priority_queue<int, vector<int>, Compare2>pq2;
	int num, a[10] = { 0 };
	cin >> num;
	while (num--) {
		memset(a, 0, 10);
		for (int i = 0; i < 10; i++) {
			cin >> a[i];
			pq1.push(a[i]);
			pq2.push(a[i]);
		}
		cout << pq2.top() << ' ' << pq1.top() << endl;
		pq2.pop(); pq1.pop();
	}
	return 0;
}
