#include<iostream>
using namespace std;
int main()
{
	int p,e,i,d;
	cin>>p>>e>>i>>d;
	int k=d+1;
	for(;(k-p)%23!=0;k++);
	for(;(k-e)%28!=0;k+=23);
	for(;(k-i)%33!=0;k+=23*28);
	cout<<k-d;
	return 0;
}
