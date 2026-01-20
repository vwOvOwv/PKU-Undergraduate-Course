#include<iostream>
#include<list>//循环链表
using namespace std;

list<int>lst;
int n,m;
int main(){
    while(cin>>n>>m&&n){
        lst.clear();
        for(int i=1;i<=n;i++){
            lst.push_back(i);
        }
        int cnt=1;
        auto now=lst.begin();
        while(lst.size()>1){
            if(!(cnt%m)){
                auto tmp=now;
                ++now;
                cnt++;
                lst.erase(tmp);
            }
            else{
                cnt++;
                ++now;
            }
            if(now==lst.end())//注意循环
                now=lst.begin();
            // for(auto k:lst)
            //     cout<<k<<' ';
            // cout<<endl;
        }
        cout<<*lst.begin()<<endl;
    }
}