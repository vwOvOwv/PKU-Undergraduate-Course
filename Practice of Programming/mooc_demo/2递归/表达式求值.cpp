/*表达式由一个项或多个项相加减组成
项由一个因子或多个因子相乘除组成
因子由一个数或一个表达式组成
*/
//本题难点在于如何划分子问题以缩小问题规模
#include<iostream>
#include<cstdlib>
#include<cstring>
using namespace std;
int factor_value();//因子求值
int term_value();//项求值
int expression_value();//表达式求值
int main()
{
	cout<<expression_value()<<endl;
	return 0;
}
int expression_value(){//求一个表达式的值
	int result=term_value();//先求第一项
	bool more=true;
	while(more){
		char op=cin.peek();//看后面还有没有项
		if(op=='+'||op=='-'){//是多项相加减
			cin.get();
			int value=term_value();
			if(op=='+')
				result+=value;
			else
				result-=value;
		}
		else
			more=false;//结束
	}
	return result;
}
int term_value(){//求一个项的值
	int result=factor_value();//先求第一个因子
	while(true){
		char op=cin.peek();
		if(op=='*'||op=='/'){//是多个因子相乘除
			cin.get();
			int value=factor_value();
			if(op=='*')
				result*=value;
			else
				result/=value;
		}
		else
			break;
	}
	return result;
}
int factor_value(){
	int result=0;
	char c=cin.peek();
	if(c=='('){//该因子是表达式
		cin.get();
		result=expression_value();
		cin.get();//把')'读走
	}
	else{
		while(c>='0'&&c<='9'){//是一个或连续多个数字
			result=10*result+c-'0';//计算数值
			cin.get();
			c=cin.peek();
		}
	}
	return result;
}
