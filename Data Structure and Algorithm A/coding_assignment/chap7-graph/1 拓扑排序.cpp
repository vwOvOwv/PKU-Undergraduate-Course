#include<iostream>
#include<vector>
#include<queue>
using namespace std;

int calInDegree(vector<vector<int> >Graph,vector<int>visited,int v,int n){
    int cnt=0;
    for(int i=1;i<=n;i++){
        if(Graph[i][v])
            cnt++;
    }
    return cnt;
}

void topSort(vector<vector<int> >Graph,int n){//不同版本的拓扑排序
    queue<int>q;
    vector<int>visited(n+1,0);
    vector<int>rec;
    for(int i=1;i<=n;i++){
        if(calInDegree(Graph,visited,i,n)==0){//只找一个入度为0的顶点
            q.push(i);
            break;
        }
    }
    while(!q.empty()){
        int p=q.front();
        q.pop();
        visited[p]=1;
        rec.push_back(p);
        for(int j=1;j<=n;j++){
            if(!visited[j]&&Graph[p][j]){
                Graph[p][j]=0;
                if(calInDegree(Graph,visited,j,n)==0)
                    q.push(j);
            }
        }
        if(q.empty()){//继续尝试找一个入度为0的顶点
            for(int i=1;i<=n;i++){
                if(!visited[i]&&!calInDegree(Graph,visited,i,n)){
                    q.push(i);
                    break;
                }
            }
        }
    }
    int size=rec.size();
    for(int i=0;i<size;i++){
        cout<<'v'<<rec[i];
        if(i<size-1)
            cout<<' ';
    }
}

int main(){
    int v,a;
    cin>>v>>a;
    vector<vector<int> >Graph(v+1,vector<int>(v+1,0));
    int v1,v2;
    for(int i=0;i<a;i++){
        cin>>v1>>v2;
        Graph[v1][v2]=1;
    }
    topSort(Graph,v);
}