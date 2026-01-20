#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int merge(int*rec,int n,int left,int right,int mid){
    int cnt=0;
    int*tmp=new int[n];
    for(int i=left;i<=right;i++){
        tmp[i]=rec[i];
    }
    int index=left,index1=left,index2=mid+1;
    while(index1<=mid&&index2<=right){
        while(index2<=right&&tmp[index1]>tmp[index2])
            rec[index++]=tmp[index2++];
        cnt+=index2-mid-1;
        if(index2<=right)
            rec[index++]=tmp[index1++];
    }
    if(index1<=mid){
        cnt+=(mid-index1)*(right-mid);
        while(index1<=mid)
            rec[index++]=tmp[index1++];
    }
    else{
        while(index2<=right)
            rec[index++]=tmp[index2++];
    }
    delete[]tmp;
    return cnt;
}

int mergeSort(int*rec,int n,int left,int right){
    if(left>=right)
        return 0;
    int mid=left+(right-left)/2;
    int cnt=0;
    cnt+=mergeSort(rec,n,left,mid);
    cnt+=mergeSort(rec,n,mid+1,right);
    cnt+=merge(rec,n,left,right,mid);
    return cnt;
}

int main(){
    int n;
    while(cin>>n&&n){
        int*rec=new int[n];
        for(int i=0;i<n;i++)
            cin>>rec[i];
        cout<<mergeSort(rec,n,0,n-1)<<endl;
    }
}