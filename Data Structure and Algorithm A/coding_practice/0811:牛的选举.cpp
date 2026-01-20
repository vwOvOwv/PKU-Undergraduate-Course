#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

class Node{
public:
    long long int round1,round2;
    int index;
};

bool Compare1(Node&a,Node&b){
    return a.round1>b.round1;
}

bool Compare2(Node&a,Node&b){
    return a.round2>b.round2;
}

int main(){
    int n,k;
    cin>>n>>k;
    vector<Node>rec(n);
    for(int i=0;i<n;i++){
        cin>>rec[i].round1>>rec[i].round2;
        rec[i].index=i+1;
    }
    sort(rec.begin(),rec.end(),Compare1);
    sort(rec.begin(),rec.begin()+k,Compare2);
    cout<<rec[0].index<<endl;
    return 0;
}