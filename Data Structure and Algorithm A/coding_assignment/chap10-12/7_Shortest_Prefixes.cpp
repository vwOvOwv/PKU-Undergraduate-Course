#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include<cstring>
using namespace std;

const int MAX=1e5;

vector<string>s(1200);
int trie[MAX][30];
int vis[MAX];       
int cnt=1;
void build_trie(string s)
{
    int root = 0;
    for(int i=0;i<s[i];i++){
        int next = s[i]-'a';
        if(!trie[root][next])
            trie[root][next]=cnt++;
        root=trie[root][next];
        vis[root]++;
    }
}

string query(string s) 
{
    string ans;
    int root=0;
    for(int i=0;i<s[i];i++){
        if(vis[root]==1)
            return ans;
        int next=s[i]-'a';
        ans+=s[i];
        root=trie[root][next];
    }
    return ans;
}
int main()
{
    memset(trie,0,sizeof(trie));
    memset(vis,0,sizeof(vis));
    int n=0;
    while(cin>>s[n]&&s[n][0]!=EOF){
        build_trie(s[n]);
        n++;
    }
    for(int i=0;i<n;i++)
        cout << s[i] << " "<< query(s[i]) << endl;
}