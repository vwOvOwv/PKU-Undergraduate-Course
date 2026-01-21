#include <iostream>
#include<algorithm>
using namespace std;

int main(){
	string ans;
	char c,before='\0';
	bool isInNote=0,isInString=0;
	while((c=cin.get())!=EOF){//逐个字符读入，分类讨论
		if(c=='\"'){
			if(!isInNote&&!isInString&&before!='\\'){//不在注释里也不在字符串里且不是'"'，说明是字符串的开始
				isInString=1;
				ans.push_back(c);
			}
			else if(!isInNote&&isInString&&before!='\\'){//不在注释里但在字符串里且不是'"'，说明是字符串的结尾
				isInString=0;
				ans.push_back(c);
			}
			else if(!isInNote&&before=='\\')//不在注释里且前一个字符是转义字符，isInString状态不变
				ans.push_back(c);
		}
		else if(c=='*'&&before=='/'){
			if(!isInNote&&!isInString){//没有在字符串中也不在注释中，说明是注释的开始
				isInNote=1;
				ans.pop_back();//要把前面的‘/’取出
			}
			else
				ans.push_back(c);//其他情况都不是有效注释
		}
		else if(c=='/'&&before=='*'){
			if(!isInString&&isInNote)//不在字符串中且在注释中，说明是注释的结束（不支持嵌套注释）
				isInNote=0;
			else
				ans.push_back(c);//其他情况都不是有效注释
		}
		else{
			if(!isInNote)//其他字符只要不在注释里都输出
				ans.push_back(c);
		}
		before=c;
	}
	cout<<ans;
	return 0;
}