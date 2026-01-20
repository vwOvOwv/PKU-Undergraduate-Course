#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<set>
#include<map>

using namespace std;

unordered_map<char,char>mem;
map<string,int>rec;

int main(){
    for(char i='A';i<='Y';i++){
        if(i>='A'&&i<='C')
            mem[i]='2';
        else if(i>='D'&&i<='F')
            mem[i]='3';

        else if(i>='G'&&i<='I')
            mem[i]='4';
        else if(i>='J'&&i<='L')
            mem[i]='5';
        else if(i>='M'&&i<='O')
            mem[i]='6';
        else if(i>='P'&&i<='S')
            mem[i]='7';
        else if(i>='T'&&i<='V')
            mem[i]='8';
        else if(i>='W'&&i<='Y')
            mem[i]='9';
    }

    int n;
    string s;
    cin>>n;
    for(int i=0;i<n;i++){
        cin>>s;
        string tmp;
        int size=s.size();
        for(int j=0;j<size;j++){
            if(s[j]=='-')
                continue;
            if(s[j]>='A'&&s[j]<='Z')
                tmp.push_back(mem[s[j]]);
            else
                tmp.push_back(s[j]);
        }
        tmp.insert(3,"-");
        rec[tmp]++;
    }
    bool found=false;
    for(auto seq:rec){
        int cnt=seq.second;
        if(cnt>1){
            found=true;
            cout<<seq.first<<' '<<cnt<<endl;
        }
    }
    if(!found){
        cout<<"No duplicates."<<endl;
    }
}