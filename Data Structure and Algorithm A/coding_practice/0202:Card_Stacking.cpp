//模拟
#include<iostream>
#include<queue>
#include<algorithm>
#include<set>

using namespace std;

int n,k,p;
queue<int>deck;
set<int>ans;
int main(){
    cin>>n>>k>>p;
    for(int i=1;i<=k;i++)
        deck.push(i);
    int now=1;
    while(!deck.empty()){
        if(now%n==0){
            ans.insert(deck.front());
        }
        now++;
        deck.pop();
        for(int i=0;i<p;i++){
            int tmp=deck.front();
            deck.push(tmp);
            deck.pop();
        }
    }
    auto kk=ans.begin();
    for(;kk!=ans.end();++kk){
        cout<<(*kk)<<endl;
    }
    return 0;
}