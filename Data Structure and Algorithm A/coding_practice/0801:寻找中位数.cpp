#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int main(){
    int n;
    cin>>n;
    vector<int>rec(n);
    for(int i=0;i<n;i++){
        cin>>rec[i];
    }
    sort(rec.begin(),rec.end());
    cout<<rec[(n-1)/2]<<endl;
    return 0;
}