#include<iostream>
#include<algorithm>
#include<map>
#include<climits>
using namespace std;

typedef multimap<int, int> vip;
int main()
{
	vip member;
	member.insert(make_pair(1000000000, 1));
	int n, id, force;
	cin >> n;
	while (n--) {
		multimap<int, int>::iterator i, p, ptr, j, tmp1, tmp2;
		cin >> id >> force;
		p = member.insert(make_pair(force, id));
		int min_id = INT_MAX;
		if (p == member.begin()) {//p在开头则只能向右找
			p++;i = p;p--;
			j = member.find(i->first);
			for (; j != member.end() && j->first == i->first; j++) {
				if (j->second < min_id) {
					min_id = j->second;
					ptr = j;
				}
			}
		}
		else if (p == --member.end()) {//p在末尾则只能向左找
			p--; i = p; p++;
			j = member.find(i->first);
			for (; j != p && j->first == i->first; j++) {
				if (j->second < min_id) {
					min_id = j->second;
					ptr = j;
				}
			}
		}
		else {
			p--; tmp1 = p; p++;
			p++; tmp2 = p; p--;
			int k = abs(tmp1->first - p->first) - abs(tmp2->first - p->first);
			if (k > 0) {//向右找
				j = member.find(tmp2->first);
				for (; j != member.end() && j->first == tmp2->first; j++) {
					if (j->second < min_id) {
						min_id = j->second;
						ptr = j;
					}
				}
			}
			else if (k < 0) {//向左找
				j = member.find(tmp1->first);
				for (; j != p && j->first == tmp1->first; j++) {
					if (j->second < min_id) {
						min_id = j->second;
						ptr = j;
					}
				}
			}
			else {//左右都找
				j = member.find(tmp1->first);
				for (; j != member.end(); j++) {
					if (j == p)
						continue;
					if (abs(j->first - p->first) != abs(tmp2->first - p->first))
						break;
					if (j->second < min_id) {
						min_id = j->second;
						ptr = j;
					}
				}

			}
		}
		cout << p->second << ' ' << ptr->second << endl;
	}
	return 0;
}
