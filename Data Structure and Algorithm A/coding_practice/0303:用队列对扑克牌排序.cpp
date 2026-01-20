#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

vector<vector<int> >color(4);
vector<vector<char> >num(10);

int n;

int main(){
    cin>>n;
    cin.get();//读掉\n
    char x;int y;
    for(int i=0;i<n;i++){
        char c=cin.get();
        x=c;
        c=cin.get();
        y=c-'0';
        num[y].push_back(x);
        color[x-'A'].push_back(y);
        cin.get();
    }
    for(int i=1;i<=9;i++){
        cout<<"Queue"<<i<<':';
        for(int j=0;j<num[i].size();j++){
            cout<<num[i][j]<<i;
            if(j!=num[i].size()-1)
                cout<<' ';
        }
        cout<<endl;
    }
    for(int i=0;i<4;i++){
        sort(color[i].begin(),color[i].end());
        cout<<"Queue"<<char(i+'A')<<':';
        for(int j=0;j<color[i].size();j++){
            cout<<char(i+'A')<<color[i][j];
            if(j!=color[i].size()-1)
                cout<<' ';
        }
        cout<<endl;
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<color[i].size();j++){
            cout<<char(i+'A')<<color[i][j];
            if(i==3&&j==color[i].size()-1){

            }
            else
                cout<<' ';
        }
    }
    cout<<endl;
    return 0;
}

