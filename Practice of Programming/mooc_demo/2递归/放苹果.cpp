/*
设i个苹果放在k个盘子里的放法总数是f(i,k),则：
k>i时,f(i,k)=f(i,i）
k<=i时，总放法=有盘子为空的放法+没盘子为空的做法
f(i,k)=f(i,k-1)（至少有一个为空）+f(i-k,k)（必有一步要先在每个盘子里放一个苹果）
*/
#include<iostream>
using namespace std;
int f(int m,int n){//m个苹果n个盘子
	if(n>m){
		return f(m,m);
	}//之后n<=m
	if(m==0)
		return 1;//没有苹果但有盘子，不放苹果，这也是一种放法！
	if(n==0)
		return 0;
	return f(m,n-1)+f(m-n,n);//m-n非负
}
int main()
{
	int t,m,n;
	cin>>t;
	while(t--){
		cin>>m>>n;
		cout<<f(m,n)<<endl;
	}
	return 0;
}
