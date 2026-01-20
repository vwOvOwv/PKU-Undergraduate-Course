#include<iostream>
#include<cstring>
#include<algorithm>

using namespace std;

int cnt[110];
int peach[110];
int bee[110];
int total;
int main()
{
	while (cin >> total) {
		if (total == -1)
			break;
		memset(peach, 0, sizeof(peach));
		memset(bee, 0, sizeof(bee));
		memset(cnt, 0, sizeof(cnt));
		int num = 1;
		while (cin >> peach[num] >> bee[num]) {
			if (peach[num] == -1 && bee[num] == -1) {
				break;
			}
			num++;
		}
		for (int i = 1; i < num; i++) {
			int tmp = total;
			for (int j = i; j < num; j++) {
				if (tmp >= bee[j]) {
					tmp -= bee[j];
					cnt[i] += peach[j];
				}
				else
					break;
			}
		}
		cout << *max_element(cnt + 1, cnt + num) << endl;
	}
	return 0;
}
