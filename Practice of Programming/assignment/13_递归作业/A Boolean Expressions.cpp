/*
给出一个中缀表达式字符串s，转换为一个后缀表达式数组postexp（数组中的每一项要么表示一个操作数，要么表示一个运算符），进而可以对表达式进行求值。
在中缀表达式和后缀表达式中，操作数的相对顺序是不变的，只是操作符的位置发生了改变，并去掉了括号，在转换的过程中，需要考虑运算符的优先级（只考虑小括号和+ - * / 运算），这里采用哈希表的方式设置运算符优先级（数字越小，优先级越高，数字相同优先级相同），并设置一个符号栈 op 保存运算符。逐步向后遍历中缀表达式s，转换过程如下：

1）若当前的字符是数字，则连同其后面连续的数字作为一个整体操作数，以字符串的形式存入到后缀表达式数组中postexp中作为一项；

2）若当前字符是左括号‘（’，则直接将其压入运算符栈op中；

3）若当前字符是右括号‘）’，则将运算符栈中的运算符依放到后缀表达式数组中，每个运算符作为一项，直到遇到一个左括号‘（’，将该左括号出栈但不加入后缀表达式；

4）若当前字符是运算符，则将从栈顶开始优先级比当前运算符高或同优先级的依次出栈并加入后缀表达式中，直至 栈为空 | 栈顶字符为‘（’ | 栈顶运算符优先级比当前运算符低，然后将当前运算符压栈；

5）遍历完中缀表达式之后，将栈中剩下的运算符依次加入后缀表达式末尾。
*/
#include<iostream>
#include<stack>
#include<string>
using namespace std;
string tran(string s) {
	stack<char>op;
	stack<char>num;
	string ans;
	int flag = 0;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '(')
			op.push('(');
		else if (s[i] == '|' || s[i] == '&') {
			while (!op.empty()&&op.top()!='(') {
				ans += op.top();
				op.pop();
			}
			op.push(s[i]);

		}
		else if (s[i] == ')') {
			while (!op.empty() && op.top() != '(') {
				ans += op.top();
				op.pop();
			}
			op.pop();
		}
		else if (s[i] == '!')
			op.push('!');
		else if (s[i] == 'V' || s[i] == 'F') {
			if (ans.empty())
				ans = s[i];
			else
				ans += s[i];
		}
	}
	while (!op.empty()) {
		ans += op.top();
		op.pop();
	}
	return ans;
}
char cal(string s) {
	stack<bool>num;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == 'V')
			num.push(true);
		else if (s[i] == 'F')
			num.push(false);
		else if (s[i] == '|' || s[i] == '&') {
			bool t1 = num.top();
			num.pop();
			bool t2 = num.top();
			num.pop();
			if (s[i] == '|') {
				num.push(t1 || t2);
			}
			else {
				num.push(t1 && t2);
			}
		}
		else if (s[i] == '!') {
			bool t = num.top();
			num.pop();
			num.push(!t);
		}
	}
	if (num.top())
		return 'V';
	else
		return 'F';
}
int main()
{
	string s;
	int cnt = 0;
	while (getline(cin, s)) {
		cnt++;
		cout << "Expression " << cnt << ": ";
		string m = tran(s);
		cout << cal(m) << endl;
	}
	return 0;
}
