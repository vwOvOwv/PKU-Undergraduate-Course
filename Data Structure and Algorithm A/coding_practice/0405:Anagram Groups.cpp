#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<unordered_map>
#include<set>
#include<map>

using namespace std;

unordered_map<string,set<string> >rec;
unordered_map<string,int>cnt;
map<int,vector<string> >_cnt;
unordered_map<string,string>mem;

int main(){
    string s;
    while(cin>>s){
        string tmp=s;
        sort(tmp.begin(),tmp.end());
        rec[tmp].insert(s);
        cnt[tmp]++;
    }
    for(auto node:cnt){
        _cnt[node.second].push_back(node.first);
    }
    int cur=0;
    auto l=_cnt.rbegin();
    for(;l!=_cnt.rend();++l){
        int size=l->second.size();
        for(int i=0;i<size;i++){
            string tmp=l->second[i];
            l->second[i]=*rec[tmp].begin();
            mem[l->second[i]]=tmp;
        }
        sort(l->second.begin(),l->second.end());
        for(int i=0;i<size;i++){
            cout<<"Group of size "<<l->first<<": ";
            string tmp=mem[l->second[i]];
            for(auto word:rec[tmp]){
                cout<<word<<' ';
            }
            cout<<'.'<<endl;
            cur++;
            if(cur>=5)
                break;
        }
        if(cur>=5)
            break;
    }
}