#include<iostream>
using namespace std;
int visited[30][50]={0};
int ways(int i,int j,int n){
	if(n==0)
		return 1;//不能返回0，表示这是一种可行的走法
	int num=0;
	visited[i][j]=1;
	if(visited[i+1][j]==0)
		num+=ways(i+1,j,n-1);
	if(visited[i][j-1]==0)
		num+=ways(i,j-1,n-1);
	if(visited[i][j+1]==0)
		num+=ways(i,j+1,n-1);
	visited[i][j]=0;//回溯
	return num;
}
int main()
{
	int n;
	cin>>n;
	cout<<ways(0,25,n)<<endl;
	return 0;
}
