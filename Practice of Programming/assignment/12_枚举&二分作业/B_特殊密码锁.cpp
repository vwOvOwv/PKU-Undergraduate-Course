//特殊密码锁
#include<iostream>
#include<string>
#include<cstring>
#include<climits>
using namespace std;
int main()
{
	char ori[30];//原始字符串
	char des[30];//目标字符串
	char tmp[30];
	cin>>ori>>des;
	strcpy(tmp,ori);
	int cnt=0;
	int min_cnt=INT_MAX;//最少按按钮操作次数
	int size=0;
	for(;ori[size]!='\0';size++);//求字符串长度，不能用sizeof()
	for(int i=0;i<size;i++){
		if(i==0){//按第一个按钮，之后的每个按钮是否需要按都随之确定下来
			tmp[0]^=1;
			tmp[1]^=1;//^1即可改变状态
			cnt++;
		}
		else if(i<size-1) {//中间的按钮
			if(tmp[i-1]!=des[i-1]){//前一个字符不符合要求，那必须要按当前按钮，否则就没有机会改变前一个字符了
				tmp[i]^=1;
				tmp[i-1]^=1;
				tmp[i+1]^=1;
				cnt++;
			}
		}
 		else{
			if(tmp[i-1]!=des[i-1]){//最后的按钮
				tmp[i]^=1;//最后一个字符并不能保证与目标字符串最后一个字符串相同
				tmp[i-1]^=1;
				cnt++;
			}
		}
	}
	if(strcmp(tmp,des)==0&&cnt<min_cnt)//符合条件
		min_cnt=cnt;
	//考虑不按动第一个按钮的情况
	cnt=0;
	strcpy(tmp,ori);
	for(int i=0;i<size;i++){
		if(i==0){
			tmp[0]^=0;
			tmp[1]^=0;
		}
		else if(i<size-1){
			if(tmp[i-1]!=des[i-1]){
				tmp[i]^=1;
				tmp[i-1]^=1;
				tmp[i+1]^=1;
				cnt++;
			}
		}
		else{
			if(tmp[i-1]!=des[i-1]){
				tmp[i]^=1;
				tmp[i-1]^=1;
				cnt++;
			}
		}
	}
	if(strcmp(tmp,des)==0&&cnt<min_cnt)
		min_cnt=cnt;
	if(min_cnt!=INT_MAX)//有合适答案
		cout<<min_cnt;
	else
		cout<<"impossible";
 	return 0;
}
