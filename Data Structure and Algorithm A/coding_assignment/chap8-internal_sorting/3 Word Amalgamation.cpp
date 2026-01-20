#include<iostream>
#include<algorithm>
#include<set>
#include<string>
#include<vector>
using namespace std;

//stl暴力破解

set<string>rec;

int main(){
    string s;
    while(cin>>s){
        if(s=="XXXXXX")
            break;
        rec.insert(s);
    }
    while(cin>>s){
        if(s=="XXXXXX")
            return 0;
        vector<string>ans;
        sort(s.begin(),s.end());
        do{
            if(rec.find(s)!=rec.end())
                ans.push_back(s);
        }while(next_permutation(s.begin(),s.end()));//枚举所有组合
        if(!ans.empty()){
            int n=ans.size();
            for(int i=0;i<n;i++){
                cout<<ans[i]<<endl;
            }
        }
        else{
            cout<<"NOT A VALID WORD"<<endl;
        }
        cout<<"******"<<endl;
    }
    return 0;
}
