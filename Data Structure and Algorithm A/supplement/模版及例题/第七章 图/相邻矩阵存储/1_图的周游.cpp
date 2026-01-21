#include<iostream>
#include<vector>
#include<queue>

using namespace std;

void visit(int n){
    cout<<n<<' ';
}

vector<int>visited(5,0);
vector<vector<int> >a(5,vector<int>(5,0));

void dfs(int v,int n){
    visited[v]=1;
    visit(v);
    for(int j=0;j<n;j++){
        if(a[v][j]&&!visited[j]){
            dfs(j,n);
        }
    }
}

void dfs_traverse(int n){
    for(auto& ele:visited)
        ele=false;
    for(int i=0;i<n;i++){
        if(!visited[i]){
            dfs(i,n);
        }
    }
}

void bfs(int v,int n){
    visited[v]=1;
    visit(v);
    queue<int>q;
    q.push(v);
    while(!q.empty()){
        int p=q.front();
        q.pop();
        for(int i=0;i<n;i++){
            if(a[p][i]&&!visited[i]){
                visit(i);
                q.push(i);
                visited[i]=1;
            }
        }
    }
}

void bfs_traverse(int n){
    for(auto& ele:visited)
        ele=false;
    for(int i=0;i<n;i++){
        if(!visited[i])
            bfs(i,n);
    }
}

int main(){
    a[0][1]=a[0][2]=a[0][4]=1;
    a[1][2]=1;
    a[2][3]=a[2][4]=1;
    dfs_traverse(5);//不要直接对点搜索，要考虑图不连通的情况
    cout<<endl;
    bfs_traverse(5);
    cout<<endl;
    return 0;
}