#include<iostream>
#include<vector>

using namespace std;

class Dist{//边
public:
    int index;
    int length;
    int pre;
    Dist(int i,int l,int p):index(i),length(l),pre(p){}
    Dist(){}
};

vector<vector<int> >Graph(7,vector<int>(7,0));//图

void print(vector<Dist>v){
    int n=v.size();

    cout<<"index ";
    for(int i=0;i<n;i++)
        cout<<v[i].index<<' ';
    cout<<endl;

    cout<<"length ";
    for(int i=0;i<n;i++)
        cout<<v[i].length<<' ';
    cout<<endl;

    cout<<"pre ";
    for(int i=0;i<n;i++)
        cout<<v[i].pre<<' ';
    cout<<endl;
}

int nextVertex(vector<Dist>rec,vector<bool>visited,int n){//计算下一个取出的顶点
    int ans=-1;
    int mini=INT_MAX;
    for(int i=0;i<n;i++){
        if(!visited[i]&&rec[i].length<mini){//注意!visited[i]
            ans=i;
            mini=rec[i].length;
        }
    }
    return ans;
}

//思路：Dijkstra框架，每次找起点已经在MST中而终点未在的边，找到权重最小的加入MST
void Prim(int n,int s){
    vector<bool>visited(n,0);
    vector<Dist>rec(n);
    vector<Dist>MST;
    for(int i=0;i<n;i++){//初始化rec
        rec[i].index=i;
        rec[i].length=INT_MAX;
        rec[i].pre=s;
    }
    rec[s].length=0;
    visited[s]=true;
    int v=s;
    for(int i=0;i<n-1;i++){//找到n-1条边即建成MST
        for(int j=0;j<n;j++){//寻找合适的边
            if(Graph[v][j]){
                if(!visited[j]&&rec[j].length>Graph[v][j]){//注意更新距离的方式与Dijkstra不同 !visited[j]
                    rec[j].length=Graph[v][j];
                    rec[j].pre=v;
                }
            }
        }
        v=nextVertex(rec,visited,n);//找到最小的边
        if(v==-1)//没有了，说明图是非连通图
            return;
        visited[v]=true;//标记，准备进入下一次循环
        MST.push_back(Dist(rec[v].index,rec[v].length,rec[v].pre));//向MST加入边
    }
    print(MST);
}
int main(){
    Graph[0][4]=1,Graph[0][1]=20;
    Graph[1][0]=20,Graph[1][3]=4,Graph[1][2]=6;
    Graph[2][1]=6,Graph[2][6]=2;
    Graph[3][1]=4,Graph[3][6]=8,Graph[3][5]=12;
    Graph[4][0]=1,Graph[4][5]=15;
    Graph[5][3]=12,Graph[5][4]=15,Graph[5][6]=10;
    Graph[6][5]=10,Graph[6][3]=8,Graph[6][2]=2;
    Prim(7,0);
    return 0;
}