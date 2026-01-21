#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
int n;
int main()
{
	scanf("%d", &n);
	for (int a = 2; a <= n; a++) {
		for (int b = 2; b <= a; b++) {
			for (int c = b; c <= a; c++) {
				for (int d = c; d <= a; d++) {
					if (a * a * a == b * b * b + c * c * c + d * d * d) {
						printf("Cube = %d, Triple = (%d,%d,%d)\n", a, b, c, d);
					}
				}
			}
		}
	}
	return 0;
}
