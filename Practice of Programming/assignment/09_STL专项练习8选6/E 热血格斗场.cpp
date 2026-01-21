#include<iostream>
#include<algorithm>
#include<map>
using namespace std;

typedef map<int, int> vip;

int main()
{
	vip member;
	member.insert(make_pair(1000000000, 1));
	int n, id, force;
	cin >> n;
	while (n--) {
		map<int, int>::iterator i, p, tmp1, tmp2;
		map<int, int>::iterator p1, p2;
		cin >> id >> force;
		member.insert(make_pair(force, id));
		i = member.find(force);
		i = member.lower_bound(force);
		if (i == member.begin()) {
			i++;
			p = i;
			i--;//注意要维护i的状态
		}
		else if (i == --member.end()) {
			i--;
			p = i;
			i++;
		}
		else {
			tmp1 = i;
			tmp1--;
			tmp2 = i;
			tmp2++;
			p1 = tmp1;
			p2 = tmp2;
			if (abs(i->first - p1->first) <= abs(i->first - p2->first))//"<="很重要
				p = p1;
			else
				p = p2;
		}
		cout << i->second << ' ' << p->second << endl;
	}
	return 0;
}
