//ººÅµËşÎÊÌâ
#include<iostream>
using namespace std;
void Hanoi(int n,char sta,char mid,char des){
	if(n==1){
		cout<<sta<<" -> "<<des<<endl;
		return;
	}
	else{
		Hanoi(n-1,sta,des,mid);
		cout<<sta<<" -> "<<des<<endl;
		Hanoi(n-1,mid,sta,des);
		return;
	}
}
int main()
{
	int n;
	cin>>n;
	Hanoi(n,'A','B','C');
	return 0;
}
