#include<iostream>
#include<vector>
#include<random>
#include<time.h>

using namespace std;

void bucketSort(vector<int>&rec,int maxi){
    vector<int>tmp=rec;
    int n=rec.size();
    vector<int>cnt(maxi+1,0);//计数器
    for(int i=0;i<n;i++)//对集合中所有值出现的次数计数
        cnt[rec[i]]++;
    for(int i=1;i<=maxi;i++)//求出每个值排好序后的结束为止
        cnt[i]+=cnt[i-1];
    for(int i=n-1;i>=0;i--){//倒着排保持稳定性！
        rec[--cnt[tmp[i]]]=tmp[i];//注意理解！！！
    }
}

int main(){
    vector<int>rec(10);
    srand(time(nullptr));
    for(int i=0;i<10;i++){
        rec[i]=rand()%20;
        cout<<rec[i]<<' ';
    }
    cout<<endl;
    bucketSort(rec,19);
    for(int i=0;i<10;i++)
        cout<<rec[i]<<' ';
    cout<<endl;
    return 0;
}