#include<iostream>
#include<cmath>
#include<iomanip>
using namespace std;
double f(double x){
	return x*x*x*x*x-15*x*x*x*x+85*x*x*x-225*x*x+274*x-121;//把函数看清楚
}
double BinarySearch(){
	double L=1.5,R=2.4;
	double mid=L+(R-L)/2;
	while(abs(f(mid))>1e-7){
		mid=L+(R-L)/2;
		if(f(mid)>0){
			L=mid;
		}
		else
			R=mid;
	}
	return mid;
}
int main(){
	cout<<fixed<<setprecision(6)<<BinarySearch()<<endl;
}
