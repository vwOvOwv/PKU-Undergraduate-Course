#include<iostream>
using namespace std;

int f(int m,int n){
    if(n==0)return 0;
    if(m==0)return 1;
    if(m<n)
        n=m;
    return f(m,n-1)+f(m-n,n);        
}
int main(){
    int t,m,n;
    cin>>t;
    while(t--){
        cin>>m>>n;
        cout<<f(m,n)<<endl;
    }
    return 0;
}