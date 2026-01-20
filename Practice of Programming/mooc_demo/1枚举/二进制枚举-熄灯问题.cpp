//利用二进制数枚举

#include<iostream>
#include<string>
#include<cstring>
#include<memory>
using namespace std;
char oriLights[5];
char lights[5];
char result[5];
//一个char为1个字节，8个二进制位
int GetBit(char c,int i){//获取c第的第i位
	return (c>>i)&1;
}
void SetBit(char&c,int i,int v){//把c的第i位改为v
	if(v){
		c|=(1<<i);
	}
	else{
		c&=~(1<<i);
	}
}
void FlipBit(char& c,int i){//把c的第i位翻转
	c^=(1<<i);
}
void OutputResult(char result[]){//输出结果
	for(int i=0;i<5;i++){
		for(int j=0;j<6;j++){
			cout<<GetBit(result[i],j);
			if(j<5)
			cout<<' ';
		}
		cout<<endl;
	}
}

int main()
{
	for(int i=0;i<5;i++){
		for(int j=0;j<6;j++){
			int s;
			cin>>s;
			SetBit(oriLights[i],j,s);
		}
	}
	for(int n=0;n<64;n++){//每一行都有2^6种枚举方式
		int switchs=n;
		memcpy(lights,oriLights,sizeof(oriLights));//每次要将lights恢复成原来状态
		for(int i=0;i<5;i++){
			result[i]=switchs;
			for(int j=0;j<6;j++){
				if(GetBit(switchs,j)){//是1则要改变对应的开关状态
					if(j>0)
						FlipBit(lights[i],j-1);
					FlipBit(lights[i],j);
					if(j<5)
						FlipBit(lights[i],j+1);
				}
			}
			if(i<4)
				lights[i+1]^=switchs;
			switchs=lights[i];//根据上一行即可确定下一行
		}
		if(lights[4]==0){
			OutputResult(result);
			break;
		}
	}
}

