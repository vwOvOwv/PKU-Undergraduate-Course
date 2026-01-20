#include<iostream>
#include<cmath>
using namespace std;
int n;
int queen_pos[100];
void n_queen(int k){//在0~k-1行皇后已经摆好的情况下摆第k行及其后面的皇后
	if(k==n){//n个皇后已经摆好
		for(int i=0;i<n;i++)
			cout<<queen_pos[i]+1<<' ';
		cout<<endl;
		return;
	}
	for(int i=0;i<n;i++){
		int j;
		for(j=0;j<k;j++){//和已经摆好的k个皇后的位置比较，看是否冲突
			if(queen_pos[j]==i||abs(k-j)==abs(queen_pos[j]-i))//冲突，则试下一个位置
				break;
		}
		if(j==k){
			queen_pos[k]=i;
			n_queen(k+1);
		}
	}
}
int main()
{
	cin>>n;
	n_queen(0);//从第零行开始摆放
	return 0;
}
