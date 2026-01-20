#include<iostream>
#include<vector>
#include<random>
#include<time.h>

using namespace std;

void radixSort(vector<int>&rec,int d,int r){//d为排序码分成的段数，r是基数（类似于maxi）
    int n=rec.size();
    int radix=1;
    for(int i=0;i<d;i++){//相当于d次桶排序
        vector<int>tmp=rec;
        vector<int>cnt(r,0);
        for(int j=0;j<n;j++)
            cnt[(tmp[j]/radix)%r]++;//注意取模技巧
        for(int j=1;j<r;j++)
            cnt[j]+=cnt[j-1];
        for(int j=n-1;j>=0;j--)//倒着收集保持稳定，这对排序正确性十分重要
            rec[--cnt[(tmp[j]/radix)%r]]=tmp[j];
        radix*=r;//进下一位
    }
}

int main(){
    vector<int>rec(10);
    srand(time(nullptr));
    for(int i=0;i<10;i++){
        rec[i]=rand()%100+(rand()%9+1)*100;
        cout<<rec[i]<<' ';
    }
    cout<<endl;
    radixSort(rec,3,10);
    for(int i=0;i<10;i++)
        cout<<rec[i]<<' ';
    cout<<endl;
    return 0;
}