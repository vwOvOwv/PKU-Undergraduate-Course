#include<iostream>
#include<queue>
#include<vector>
using namespace std;

vector<vector<int> >Graph(9,vector<int>(9,0));
vector<int>visited(9,0);


//统计图中入度为0的点并删除其所有出边，循环此步骤
//可以判断图中是否有环

//利用队列进行拓扑排序
int calInDegree(int v,int n){//辅助函数，计算顶点入度
    int ans=0;
    for(int i=0;i<n;i++){
        if(Graph[i][v])
            ans++;
    }
    return ans;
}
void topSortByQueue(int n){
    for(auto& vertex:visited)
        vertex=0;
    queue<int>q;
    for(int i=0;i<n;i++){
        if(!calInDegree(i,n))
            q.push(i);
    }
    while(!q.empty()){
        int p=q.front();
        q.pop();
        cout<<p<<' ';
        visited[p]=1;
        for(int i=0;i<n;i++){
            if(Graph[p][i]){
                Graph[p][i]=0;
                if(!visited[i]&&!calInDegree(i,n)){//删除之后入度为0方可入队
                    q.push(i);
                }
            }
        }
    }
    for(int i=0;i<n;i++){
        if(!visited[i]){//还有没访问过的
            cout<<"There is a circle in the graph."<<endl;
            break;
        }
    }
}

//利用深度优先周游进行拓扑排序
void dfs(int v,int n,int*result,int& index){
    visited[v]=1;
    for(int i=0;i<n;i++){
        if(Graph[v][i]&&!visited[i])
            dfs(i,n,result,index);
    }
    result[index]=v;//后访问
    index++;
}
void topSortByDfs(int n){
    int*result=new int[n];
    int index=0;
    for(auto& vertex:visited)
        vertex=0;
    for(int i=0;i<n;i++){
        if(!visited[i]&&!calInDegree(i,n))
            dfs(i,n,result,index);
    }
    //不能判断是否有环
    // for(int i=0;i<n;i++){
    //     if(!visited[i]){
    //         cout<<"There is a circle in the graph."<<endl;
    //         goto loop;
    //     }
    // }
    for(int i=n-1;i>=0;i--){
        cout<<result[i]<<' ';
    }
    loop:;
}

int main(){
    Graph[0][2]=Graph[0][7]=Graph[0][4]=1;
    Graph[1][2]=Graph[1][4]=Graph[1][3]=1;
    Graph[2][3]=1;
    Graph[3][5]=Graph[3][6]=1;
    Graph[4][5]=1;
    Graph[7][8]=1;
    Graph[8][6]=1;
    topSortByQueue(9);
    cout<<endl;
    Graph[0][2]=Graph[0][7]=Graph[0][4]=1;
    Graph[1][2]=Graph[1][4]=Graph[1][3]=1;
    Graph[2][3]=1;
    Graph[3][5]=Graph[3][6]=1;
    Graph[4][5]=1;
    Graph[7][8]=1;
    Graph[8][6]=1;
    topSortByDfs(9);
    cout<<endl;
    return 0;
}