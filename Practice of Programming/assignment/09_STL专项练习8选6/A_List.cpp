#include <iostream>
#include <list>
#include <string>
#include<vector>
#include<algorithm>
using namespace std;

void new_list(int id);
void add_num(int id,int num);
void out_list(int id);
void merge_list(int id1, int id2);
void unique_list(int id);

vector<list<int>>v(10000);
int main() {
	int n;
	string str;
	int a = 0, b = 0;
	cin >> n;
	while (n--) {
		cin >> str;
		if (str == "new") {
			cin >> a;
			new_list(a);
		}
		else if (str == "add") {
			cin >> a >> b;
			add_num(a, b);
		}
		else if (str == "merge") {
			cin >> a >> b;
			merge_list(a, b);
		}
		else if (str == "out") {
			cin >> a;
			out_list(a);
		}
		else if (str == "unique") {
			cin >> a;
			unique_list(a);
		}
	}
	a = 0, b = 0;
	return 0;
}

void new_list(int id) {
	v[id].clear();
}
void add_num(int id, int num) {
	v[id].push_back(num);
}
void out_list(int id) {
	v[id].sort();
	list<int>::iterator i;
	i = v[id].begin();
	for (; i != v[id].end(); i++)
		cout << *i << ' ';
	cout << endl;
}
void merge_list(int id1, int id2) {
	if (id1 != id2) {
		v[id1].merge(v[id2]);
	}
}
void unique_list(int id) {
	v[id].sort();
	v[id].unique();//注意要先排序才能清除全部重复元素
}
