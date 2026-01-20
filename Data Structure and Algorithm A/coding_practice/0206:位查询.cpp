#include<iostream>
#include<vector>

using namespace std;

const int maxi=65536;
int n,m;
vector<int>num;
char op;
int GetBit(int u,int v){
    return (u>>v)&1;
}
int main(){
    cin>>n>>m;
    for(int i=0;i<n;i++){
        int x;
        cin>>x;
        num.push_back(x);
    }
    // for(int i=0;i<n;i++)
    //     cout<<num[i]<<' ';
    // cout<<endl;
    for(int i=0;i<m;i++){
        cin>>op;
        int t;
        if(op=='C'){
            cin>>t;
            for(int j=0;j<n;j++){
                num[j]=(num[j]%maxi+t%maxi)%maxi;
            }
        }
        else if(op=='Q'){
            cin>>t;
            int cnt=0;
            for(int j=0;j<n;j++){
                if(GetBit(num[j],t))
                    cnt++;
            }
            cout<<cnt<<endl;
        }
    }
    return 0;
}