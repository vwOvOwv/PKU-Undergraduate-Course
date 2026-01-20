#include<vector>
#include<string>
#include<iostream>
#include<algorithm>

using namespace std;

long long int rec[500010];
int n;

long long int merge(int left,int right,int mid){
    long long int cnt=0;
    int*tmp=new int[n];
    for(int i=left;i<=right;i++)
        tmp[i]=rec[i];
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
    delete[] tmp;
    return cnt;
}

long long int mergeSort(int left,int right){
    if(left>=right)
        return 0;
    int mid=left+(right-left)/2;
    long long int cnt=0;
    cnt+=mergeSort(left,mid);
    cnt+=mergeSort(mid+1,right);
    cnt+=merge(left,right,mid);
    return cnt;
}

int main(){
    while(cin>>n&&n){
        for(int i=0;i<n;i++)
            cin>>rec[i];
        cout<<mergeSort(0,n-1)<<endl;
    }
}