#include<iostream>
#include<vector>
#include<random>
#include<time.h>

using namespace std;

int selectPivot(int left,int right){
    return left+(right-left)/2;
}

int partition(vector<int>&rec,int left,int right){
    int l=left,r=right;
    int pivotValue=rec[right];
    while(l<r){
        while(rec[l]<=pivotValue&&l<r)
            l++;
        if(l<r){
            rec[r]=rec[l];
            r--;
        }
        while(rec[r]>=pivotValue&&l<r)
            r--;
        if(l<r){
            rec[l]=rec[r];
            l++;
        }
    }
    rec[l]=pivotValue;
    return l;
}

void quickSort(vector<int>&rec,int left,int right){
    if(left>=right)
        return;
    int pivot=selectPivot(left,right);//选取轴值
    swap(rec[pivot],rec[right]);//把轴值放到最后，便于利用这一个空位
    pivot=partition(rec,left,right);//分割，将轴值排好位置
    quickSort(rec,left,pivot-1);//排左子序列
    quickSort(rec,pivot+1,right);//排右子序列
}

int main(){
    vector<int>rec(10);
    srand(time(nullptr));
    for(int i=0;i<10;i++){
        rec[i]=rand()%10;
        cout<<rec[i]<<' ';
    }
    cout<<endl;
    quickSort(rec,0,10-1);
    for(int i=0;i<10;i++)
        cout<<rec[i]<<' ';
    cout<<endl;
    return 0;
}