#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<queue>

using namespace std;

priority_queue<int>Greater;
priority_queue<int,vector<int>,greater<int> >Less;

void balance(){
    if(Greater.size()>Less.size()+1){
        Less.push(Greater.top());
        Greater.pop();
    }
    else if(Greater.size()<Less.size()){
        Greater.push(Less.top());
        Less.pop();
    }
}

void push(int num){
    if(Greater.empty()){
        Greater.push(num);
        return;
    }
    int mid=Greater.top();
    if(num<mid){
        Greater.push(num);
    }
    else{
        Less.push(num);
    }
    balance();
}

int main(){
    int n;
    cin>>n;
    int num;
    for(int i=0;i<n;i++){
        cin>>num;
        push(num);
        if(i%2==0)
            cout<<Greater.top()<<endl;
    }
}