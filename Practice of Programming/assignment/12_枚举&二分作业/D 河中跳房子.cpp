#include<iostream>
#include<vector>
using namespace std;
int l,m,n;
int ri[50010];
bool judge(int mid){
	int cnt=0;
	int std=ri[0];
	for(int i=1;i<=n+1;i++){//n+1是重要的
		if(ri[i]-std<mid){
			cnt++;
		}
		else{
			std=ri[i];
		}
	}
	if(cnt>m)
		return false;
	else
		return true;
}
int main()
{
	cin>>l>>n>>m;
	for(int i=1;i<=n;i++){
		cin>>ri[i];
	}
	ri[0]=0;
	ri[n+1]=l;
	int L=0,R=l,mid;
	while(L<=R){
		mid=L+(R-L)/2;
		if(!judge(mid)){
			R=mid-1;
		}
		else
			L=mid+1;
	}
	cout<<R<<endl;
	return 0;
}
