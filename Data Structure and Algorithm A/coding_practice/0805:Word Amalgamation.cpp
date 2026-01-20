#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<set>

using namespace std;

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
            break;
        sort(s.begin(),s.end());
        bool found=0;
        do{
            if(rec.count(s)){
                cout<<s<<endl;
                found=1;
            }
        }while(next_permutation(s.begin(),s.end()));
        if(!found){
            cout<<"NOT A VALID WORD"<<endl;
        }
        cout<<"******"<<endl;
    }
}