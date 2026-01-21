#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#define _tab  "|     "
using namespace std;
vector<string>v;//储存所有文件和目录名
vector<string>layer_0;//储存根目录下的文件名，用于最后输出
int start = 0;//记录开始位置
void output(vector<string>v,int layer) {//输出格式控制
	int cnt = 0;
	sort(v.begin(), v.end());//注意文件名是有序的
	for (int i = 0; i < v.size(); i++) {
		for (int i = 0; i < layer; i++)
			cout << _tab;
		cout << v[i] << endl;
	}
}
void f(int layer) {
	vector<string>tmp;
	for (int i = start; i < v.size(); i++) {
		if (v[i][0] == 'f')
		{
			if (layer > 0)
				tmp.push_back(v[i]);
			else
				layer_0.push_back(v[i]);
		}
		else if (v[i][0] == 'd') {
			for (int i = 0; i < layer + 1; i++)
				cout << _tab;
			cout << v[i] << endl;
			start = i + 1;//从当前字符串的下一个开始
			f(layer + 1);
			i = start - 1;//注意这里要先-1，因为有i++
		}
		else if (v[i][0] == ']') {
			output(tmp, layer);
			tmp.clear();//  清空当前层的tmp，其实可有可无
			start = i + 1;//跳过已经记录过的
			return;
		}
	}
	output(layer_0, 0);//最后输出根目录下的文件
}
int main()
{
	string s;
	int cnt = 0;
	while (1) {
		cin >> s;
		if (s[0]=='*') {
			cnt++;
			cout << "DATA SET " << cnt << ':' << endl;
			cout << "ROOT" << endl;
			f(0);
			start = 0;
			v.clear();
			layer_0.clear();//重置全局变量
			cout << endl;//注意
		}
		else if (s[0] == '#') {
			break;
		}
		else {
			v.push_back(s);
		}
	}
	return 0;
}
