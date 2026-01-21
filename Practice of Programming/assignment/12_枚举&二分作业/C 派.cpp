#include<iostream>
#include<algorithm>
#include<iomanip>
#include<cmath>
using namespace std;

const double pi=acos(-1);
int main()
{
	int n,f;
	cin>>n>>f;
	int r[10010];
	for(int i=0;i<n;i++){
		cin>>r[i];
	}
	sort(r,r+n);//先排序
	double L=0;//每块体积下界
	double R=pi*r[n-1]*r[n-1];//每块体积上界
	double ans=0;//最大体积,注意不要设置成-1，因为在极端情况下最大体积可能为0.0000000...
	             //此时无法进入下方的while循环，ans没有被改变，会输出-1.000而不是0.000
    while(R-L>1e-5){//精确度要达到要求
    	int pie_num=0;
    	double mid=L+(R-L)/2;//二分查找
    	for(int i=0;i<n;i++){
    		pie_num+=int(pi*r[i]*r[i]/mid);
    	}
    	if(pie_num<f+1){//体积太大了
    		R=mid;
    	}
    	else{
    		if(mid>ans)//体积足够
				ans=mid;
    		L=mid;//尝试是否有更大的，注意这一步可能无法被执行，在循环结束后要再判断一次
    	}
    }
	cout<<fixed<<setprecision(3)<<ans<<endl;
    return 0;
 }
