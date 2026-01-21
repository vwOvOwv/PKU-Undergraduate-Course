#include<iostream>
#include<vector>
using namespace std;

//归并排序框架，注意不要使用stl，会卡常数

int merge(int*v,int n,int left,int right,int mid){
    int*tmp=new int[n];
    for(int i=left;i<=right;i++)//复制要用的一部分就好，省时间
        tmp[i]=v[i];
    int index1=left,index2=mid+1;
    int index=left;
    int cnt=0;
    while(index1<=mid&&index2<=right){
        while(index2<=right&&tmp[index1]>tmp[index2])
            v[index++]=tmp[index2++];
        cnt+=index2-mid-1;
        if(index2<=right)
            v[index++]=tmp[index1++];
    }
    if(index1<=mid){
        cnt+=(mid-index1)*(right-mid);
        while(index1<=mid)
            v[index++]=tmp[index1++];
    }
    else{
        while(index2<=right)
            v[index++]=tmp[index2++];
    }
    delete[] tmp;//一定要删掉，否则mle
    return cnt;
}

int mergeSort(int*v,int n,int left,int right){
    if(left>=right)
        return 0;
    int cnt=0;
    int mid=left+(right-left)/2;
    cnt+=mergeSort(v,n,left,mid);//求左子序列的逆序对个数
    cnt+=mergeSort(v,n,mid+1,right);//求右子序列逆序对个数
    cnt+=merge(v,n,left,right,mid);//求跨越中线的逆序对个数
    return cnt;
}

int v[20010]={0};

int main(){
    int n;
    //快读
    cin.tie(0);
    iostream::sync_with_stdio(0);
    
    while(cin>>n&&n){
        for(int i=0;i<n;i++)
            cin>>v[i];
        cout<<mergeSort(v,n,0,n-1)<<endl;
    }
    // for(int i=0;i<20000;i++)
    //     v[i]=20000-i;
    // cout<<mergeSort(v,n,0,20000-1)<<endl;
    return 0;
}