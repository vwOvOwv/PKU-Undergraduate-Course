#include<iostream>
#include<string>
#include<vector>
#include<set>

using namespace std;

int main(){
    int n;
    cin>>n;
    vector<set<string> >rec(n,set<string>());
    int length;
    for(int i=0;i<n;i++){
        cin>>length;
        string s;
        for(int k=0;k<length;k++){
            cin>>s;
            //cout<<s<<endl;//
            rec[i].insert(s);
        }
    }
    int m;
    cin>>m;
    for(int i=0;i<m;i++){
        string s;
        vector<int>ans;
        cin>>s;
        for(int j=0;j<n;j++){
            // cout<<"called"<<endl;//
            if(!rec[j].empty()&&rec[j].count(s))
                ans.push_back(j+1);
        }
        //cout<<"called"<<endl;//
        int size=ans.size();
        if(!size){
            cout<<"NOT FOUND"<<endl;
            continue;
        }
        for(int j=0;j<size;j++){
            cout<<ans[j];
            if(j<size-1)
                cout<<' ';
        }
        cout<<endl;
    }
}