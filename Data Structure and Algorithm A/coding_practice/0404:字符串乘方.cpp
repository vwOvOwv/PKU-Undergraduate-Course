#include<iostream>
#include<vector>
#include<string>

using namespace std;

int main(){
    string s;
    while(cin>>s){
        if(s==".")
            break;
        int n=s.size();
        vector<int>next(n+1,0);
        int j=0,k=-1;
        next[0]=-1;
        while(j<n){
            while(k>=0&&s[j]!=s[k])
                k=next[k];
            k++,j++;
            next[j]=k;
        }
        int seg=n-next[n];
        if(n%seg==0){
            cout<<n/seg<<endl;
        }
        else
            cout<<'1'<<endl;
    }
}