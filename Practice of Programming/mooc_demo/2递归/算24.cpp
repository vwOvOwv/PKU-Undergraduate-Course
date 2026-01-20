/*
n个数算24,必有两个数要先算，这两个数算的结果
和剩余两个数就构成了n-1个数求24的问题
*/
#include<iostream>
#include<cmath>
using namespace std;
#define EPS 1e-6
bool is_zero(double x){//浮点数比大小不能用==
	return fabs(x)<=EPS;
}
bool count24(double a[],int n){//用a中的n个数算24
	if(n==1){//所有数都要用上，因此最后只剩下1个数
		if(is_zero(a[0]-24))
			return true;
		else
			return false;
	}
	double b[5];
	for(int i=0;i<n-1;i++){
		for(int j=i+1;j<n;j++){//枚举最先算的两个数
			int m=0;
			for(int k=0;k<n;k++){
				if(k!=i&&k!=j){
					b[m]=a[k];
					m++;
				}
			}
			b[m]=a[i]+a[j];
			if(count24(b,m+1))//m+1=n-1
				return true;
			b[m]=a[i]-a[j];
			if(count24(b,m+1))
				return true;
			b[m]=a[j]-a[i];
			if(count24(b,m+1))
				return true;
			b[m]=a[i]*a[j];
			if(count24(b,m+1))
				return true;
			if(!is_zero(a[i])){//注意判断
				b[m]=a[j]/a[i];
				if(count24(b,m+1))
					return true;
			}
			if(!is_zero(a[j])){
				b[m]=a[i]/a[j];
				if(count24(b,m+1))
					return true;
			}
		}
	}
	return false;
}
int main()
{
	double a[5];
	for(int i=0;i<4;i++)
		cin>>a[i];
	if(count24(a,4))
		cout<<"yes"<<endl;
	else
		cout<<"no"<<endl;
	return 0;
}

