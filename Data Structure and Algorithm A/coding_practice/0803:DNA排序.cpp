#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

int merge(string&s,int left,int right,int mid){
    int cnt=0;
    string tmp=s;
    int index1=left,index2=mid+1,index=left;
    while(index1<=mid&&index2<=right){
        while(index2<=right&&tmp[index1]>tmp[index2])
            s[index++]=tmp[index2++];
        cnt+=index2-mid-1;
        if(index2<=right)
            s[index++]=tmp[index1++];
    }
    if(index1<=mid){
        cnt+=(mid-index1)*(right-mid);
        while(index1<=mid)
            s[index++]=tmp[index1++];
    }
    else{
        while(index2<=right)
            s[index++]=tmp[index2++];
    }
    return cnt;
}

int mergeSort(string&s,int left,int right){
    if(left>=right)
        return 0;
    int mid=left+(right-left)/2;
    int cnt=0;
    cnt+=mergeSort(s,left,mid);
    cnt+=mergeSort(s,mid+1,right);
    cnt+=merge(s,left,right,mid);
    return cnt;
}

int main(){
    int n,m;
    cin>>n>>m;
    string s,tmp;
    vector<pair<int,string> >ans;
    for(int i=0;i<m;i++){
        cin>>s;
        tmp=s;
        int cnt=mergeSort(tmp,0,n-1);
        // cout<<tmp<<endl;//
        ans.push_back(make_pair(cnt,s));
    }
    stable_sort(ans.begin(),ans.end());
    for(auto DNA:ans){
        cout<<DNA.second<<endl;
    }
}