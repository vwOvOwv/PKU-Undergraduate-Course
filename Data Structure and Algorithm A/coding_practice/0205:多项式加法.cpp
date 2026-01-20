#include <iostream>
#include <map>
using namespace std;

int main() {
	int n;
	cin >> n;
	map<int, int> a, b;
	while (n--) {
		a.clear();
		b.clear();
		int num = 0, exp = 0;
		while (cin >> num >> exp && exp >= 0) {
			if (a.find(exp) == a.end())
				a[exp] = num;
			else
				a[exp] += num;
		}
		while (cin >> num >> exp && exp >= 0) {
			if (b.find(exp) == b.end())
				b[exp] = num;
			else
				b[exp] += num;
		}
		for (auto c: b) {
			auto pos = a.find(c.first);
			if (pos == a.end()) {
				a[c.first] = c.second;
			}
			else {
				pos->second += c.second;
			}
		}
		for (auto c = a.rbegin(); c != a.rend(); ++c) {
			if (c->second != 0) {
				printf("[ %d %d ] ", c->second, c->first);
			}
		}
		printf("\n");
	}
	return 0;
}