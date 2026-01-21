#include<iostream>
#include<set>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<map>

using namespace std;

map<string,set<string> >rec;//记录所有字母相同的单词
map<string,int>cnt;//记录某一种单词的个数
map<int,vector<string> >_cnt;//记录每个size的单词

map<string,string>mem;//建立起rec中单词到“最小字典序”单词的映射

int main(){
    string s;
    while(cin>>s){
        string tmp=s;
        sort(tmp.begin(),tmp.end());//tmp是“最小字典序”单词，作为rec,cnt,_cnt中的单词“代表”
        rec[tmp].insert(s);
        cnt[tmp]++;//计数
    }
    auto str=cnt.begin();
    for(;str!=cnt.end();++str)//记录每个size的单词
        _cnt[(*str).second].push_back((*str).first);

    int cur=0;
    auto length=_cnt.rbegin();//倒着按照size从大到小的顺序找
    for(;length!=_cnt.rend();++length){
        int size=(*length).second.size();
        for(int i=0;i<size;i++){
            mem[*(rec[(*length).second[i]].begin())]=(*length).second[i];
            (*length).second[i]=*(rec[(*length).second[i]].begin());//替换，从而能够做到真正对单词排序而不是对“最小字典序”单词排序！！！
        }
        sort((*length).second.begin(),(*length).second.end());
        for(int i=0;i<size;i++){
            cout<<"Group of size "<<(*length).first<<": ";
            auto j=rec[mem[(*length).second[i]]].begin();//利用mem找到对应的“最小字典序单词”
            for(;j!=rec[mem[(*length).second[i]]].end();++j)
                cout<<(*j)<<' ';
            cout<<'.'<<endl;
            ++cur;
            if(cur>=5)//找前5个即可
                break;
        }
        if(cur>=5)
            break;
    }
    return 0;
}