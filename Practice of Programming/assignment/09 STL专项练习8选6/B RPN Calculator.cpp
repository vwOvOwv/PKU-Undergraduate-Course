#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<climits>
#include<vector>
using namespace std;

double op1(double& a, double& b) {
	return a + b;
}
double op2(double& a, double& b) {
	return a - b;
}
double op3(double& a, double& b) {
	return a * b;
}
double op4(double& a, double& b) {
	return a / b;
}
double op5(double& a, int b) {
	double tmp = 1;
	if (b >= 0) {
		for (int i = 1; i <= b; i++) {
			tmp = tmp * a;
		}
	}
	else {
		for (int i = 1; i <= -b; i++) {
			tmp = tmp / a;
		}
	}
	return tmp;
}
priority_queue<double, vector<double>, greater<double> > storage;
int c;
double num1;
vector<double>num;
void print(double& a) {
	storage.pop();
	storage.push(a);
	printf("%e\n", a);
	num.clear();
}
int main() {
	int n;
	scanf("%d", &n);
	int tmp = n;
	while (tmp--) {
		double a;
		scanf("%lf", &a);
		storage.push(a);
	}
	while ((c = cin.peek()) != EOF) {
		if (c >= '0' && c <= '9') {
			scanf("%lf", &num1);
			num.push_back(num1);
		}
		else {
			vector<double>::iterator i;
			if (c == '+') {
				i = num.end() - 1;
				*(i - 1) = op1(*(i - 1), *i);
				cin.ignore(1);
				num.erase(i);
			}
			else if (c == '-') {

				cin.ignore(1);
				int d = cin.peek();
				if (d >= '0' && d <= '9') {
					scanf("%lf", &num1);
					num.push_back(-num1);
				}
				else {
					i = num.end() - 1;
					*(i - 1) = op2(*(i - 1), *i);
					num.erase(i);
				}
			}
			else if (c == '*') {
				i = num.end() - 1;
				*(i - 1) = op3(*(i - 1), *i);
				cin.ignore(1);
				num.erase(i);
			}
			else if (c == '/') {
				i = num.end() - 1;
				*(i - 1) = op4(*(i - 1), *i);
				cin.ignore(1);
				num.erase(i);
			}
			else if (c == '^') {
				i = num.end() - 1;
				*(i - 1) = op5(*(i - 1), *i);
				cin.ignore(1);
				num.erase(i);
			}
			else if (c == '=') {
				cin.ignore(1);
				print(num[0]);
			}
			else
				cin.ignore(1);
		}
	}
	printf("\n");
	for (int i = 1; i <= n; i++) {
		if (i != n && i % 10 != 0)
			printf("%e ", storage.top());
		if (i == n)
			printf("%e", storage.top());
		if (i % 10 == 0 && i != 1 && i != n)
			printf("%e\n", storage.top());
		storage.pop();
	}
	return 0;
}
