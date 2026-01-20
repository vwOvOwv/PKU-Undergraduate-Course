#include<iostream>
#include<vector>
#include<random>
#include<time.h>

using namespace std;

void merge(vector<int>&rec,int left,int right,int mid){
    vector<int>tmp=rec;
    int index=left,index1=left,index2=mid+1;
    while(index1<=mid&&index2<=right){
        if(tmp[index1]<=tmp[index2])
            rec[index++]=tmp[index1++];
        else
            rec[index++]=tmp[index2++];
    }
    while(index1<=mid)
        rec[index++]=tmp[index1++];
    while(index2<=right)
        rec[index++]=tmp[index2++];
}

void SedgewickMerge(vector<int>&rec,int left,int right,int mid){
    vector<int>tmp(rec.size());
    for(int i=left;i<=mid;i++)
        tmp[i]=rec[i];
    for(int i=mid+1;i<=right;i++)//颠倒右子序列
        tmp[mid+right-i+1]=rec[i];
    int index=left,index1=left,index2=right;
    while(index1<=index2){//注意边界条件
        if(tmp[index1]<=tmp[index2])
            rec[index++]=tmp[index1++];
        else
            rec[index++]=tmp[index2--];
    }
}

void mergeSort(vector<int>&rec,int left,int right){
    if(left>=right)
        return;
    int mid=left+(right-left)/2;
    mergeSort(rec,left,mid);//排左子序列
    mergeSort(rec,mid+1,right);//排右子序列
    // merge(rec,left,right,mid);
    SedgewickMerge(rec,left,right,mid);//归并两个排好的序列
}

int main(){
    vector<int>rec(10);
    srand(time(nullptr));
    for(int i=0;i<10;i++){
        rec[i]=rand()%10;
        cout<<rec[i]<<' ';
    }
    cout<<endl;
    mergeSort(rec,0,10-1);
    for(int i=0;i<10;i++)
        cout<<rec[i]<<' ';
    cout<<endl;
    return 0;
}