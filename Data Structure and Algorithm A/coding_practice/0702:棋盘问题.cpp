#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

char plate[10][10]={0};
int sum;

void solve(int n,int k,int r){
    if(!k){
        sum++;
        // for(int i=0;i<n;i++){
        //     for(int j=0;j<n;j++){
        //         cout<<plate[i][j];
        //     }
        //     cout<<endl;
        // }
        return;
    }
    if(r==n+1)
        return;
    for(int i=0;i<n;i++){
        if(plate[r][i]=='#'){
            int j;
            for(j=0;j<r;j++){
                if(plate[j][i]==';')
                    break;
            }
            if(j==r){
                plate[r][i]=';';
                solve(n,k-1,r+1);
                plate[r][i]='#';
            }
        }
    }
    solve(n,k,r+1);
}

int main(){
    int n,k;
    while(cin>>n>>k&&n!=-1){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                cin>>plate[i][j];
            }
        }
        sum=0;
        solve(n,k,0);
        cout<<sum<<endl;
    }
}