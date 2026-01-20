#include<iostream>
#include<algorithm>
#include<cstdlib>
using namespace std;
int main()
{
	int n,m;
	int* p;
	cin>>n>>m;
	p=new int[n];
	for(int i=0;i<n;i++){
		cin>>p[i];
	}
	sort(p,p+n);//二分查找是有序序列算法
	//解法一
	int i;
	for(i=0;i<n;i++){
		if(binary_search(p+i+1,p+n,m-p[i])){//对每一个p[i]向后二分查找m-p[i](之前的已经查找过了)
			cout<<p[i]<<' '<<m-p[i]<<endl;
			break;
		}
	}
	if(i==n)
		cout<<"none"<<endl;
	//解法二
	i=0;
	int j=n-1;
	while(p[i]+p[j]!=m){
		if(p[i]+p[j]>m){
			j--;//p[j]大了
		}
		else
			i++;//p[i]小了
		if(i>=j)//没找到
			break;
	}
	if(i>=j)
		cout<<"none"<<endl;
	else
		cout<<p[i]<<' '<<p[j]<<endl;
	return 0;
}
