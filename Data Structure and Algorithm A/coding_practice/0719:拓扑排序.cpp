#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<queue>

using namespace std;

int calInDegree(vector<vector<int> >Graph,int n,int i){
    int cnt=0;
    for(int j=1;j<=n;j++){
        if(Graph[j][i])
            cnt++;
    }
    return cnt;
}

void topSort(vector<vector<int> >Graph,int n){
    vector<int>ans;
    queue<int>q;
    vector<int>visited(n+1,0);
    for(int i=1;i<=n;i++){
        if(!calInDegree(Graph,n,i)){
            q.push(i);
            visited[i]=1;
            break;
        }
    }
    while(!q.empty()){
        int p=q.front();
        // cout<<p<<endl;//
        q.pop();
        ans.push_back(p);
        for(int i=1;i<=n;i++){
            if(Graph[p][i]){
                Graph[p][i]=0;
                if(!visited[i]&&!calInDegree(Graph,n,i)){
                    visited[i]=1;
                    q.push(i);
                }
            }
        }
        if(q.empty()){
            for(int i=1;i<=n;i++){
                if(!visited[i]&&!calInDegree(Graph,n,i)){
                    visited[i]=1;
                    q.push(i);
                    break;
                }
            }
        }
    }
    for(int i=0;i<n;i++){
        cout<<'v'<<ans[i];
        if(i<n-1)
            cout<<' ';
    }
    cout<<endl;
}

int main(){
    int v,a;
    cin>>v>>a;
    vector<vector<int> >Graph(v+1,vector<int>(v+1,0));
    for(int i=0;i<a;i++){
        int from,to;
        cin>>from>>to;
        Graph[from][to]=1;
    }
    topSort(Graph,v);
}