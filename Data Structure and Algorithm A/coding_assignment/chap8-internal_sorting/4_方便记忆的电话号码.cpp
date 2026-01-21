#include<iostream>
#include<unordered_map>
#include<map>
#include<vector>

using namespace std;

unordered_map<char,char>fun;
map<string,int>ans;

int main(){
    //建立映射
    for(char i='A';i<='Z';i++){
        if(i>='A'&&i<='C')
            fun[i]='2';
        else if(i>='D'&&i<='F')
            fun[i]='3';
        else if(i>='G'&&i<='I')
            fun[i]='4';
        else if(i>='J'&&i<='L')
            fun[i]='5';
        else if(i>='M'&&i<='O')
            fun[i]='6';
        else if(i>='P'&&i<='S')
            fun[i]='7';
        else if(i>='T'&&i<='V')
            fun[i]='8';
        else
            fun[i]='9';
    }

    int n;
    cin>>n;
    string num;
    bool found=false;//标记是否有多次出现的号码
    for(int i=0;i<n;i++){
        cin>>num;
        string rec;
        int length=num.length();
        //进行号码标准化
        for(int i=0;i<length;i++){
            char tmp=num[i];
            if(tmp>='A'&&tmp<='Z')
                rec.push_back(fun[tmp]);
            else if(tmp>='0'&&tmp<='9')
                rec.push_back(tmp);
            if(rec.size()==3)
                rec.push_back('-');
        }
        if(ans.find(rec)!=ans.end()){//查找
            ans[rec]++;
            found=true;//有重复的，要输出
        }
        else{
            ans[rec]=1;
        }
    }
    if(found){
        map<string,int>::iterator i=ans.begin();
        for(;i!=ans.end();++i){
            if((*i).second>1)
                cout<<(*i).first<<' '<<(*i).second<<endl;
        }
    }
    else
        cout<<"No duplicates."<<endl;
    return 0;
}