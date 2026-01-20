#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<queue>
#include<set>

using namespace std;

void topSort(vector<vector<int> >&rec,vector<int>&pre,int n){
    vector<int>visited(n+1,0);
    queue<int>q;
    for(int i=1;i<=n;i++){
        if(pre[i]==0){
            q.push(i);
            visited[i]=1;
        }
    }
    while(!q.empty()){
        // for(int i=1;i<=n;i++)
        //     cout<<pre[i]<<' ';
        // cout<<endl;
        int p=q.front();
        // cout<<p<<endl;
        q.pop();
        int size=rec[p].size();
        // cout<<"size="<<size<<endl;
        for(int i=0;i<size;i++){
            pre[rec[p][i]]--;
            if(!visited[rec[p][i]]&&pre[rec[p][i]]==0){
                visited[rec[p][i]]=1;
                q.push(rec[p][i]);
            }
        }
    }
    for(int i=1;i<=n;i++){
        if(!visited[i]){
            cout<<"Yes"<<endl;
            return;
        }
    }
    cout<<"No"<<endl;
}

int main(){
    int t;
    cin>>t;
    while(t--){
        int n,m;
        cin>>n>>m;
        vector<vector<int> >rec(n+1);
        vector<int>pre(n+1);
        int from,to;
        for(int i=0;i<m;i++){
            cin>>from>>to;
            rec[from].push_back(to);
            pre[to]++;
        }
        // for(int i=1;i<=n;i++)
        //     cout<<pre[i]<<' ';
        // cout<<endl;
        topSort(rec,pre,n);
    }
}