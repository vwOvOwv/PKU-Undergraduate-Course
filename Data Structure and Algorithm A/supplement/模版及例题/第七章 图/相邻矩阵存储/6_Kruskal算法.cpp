#include<iostream>
#include<vector>
#include<queue>
using namespace std;

class TreeNode{//并查集结点
public:
    int father;
    int weight;
};

class Edge{//边的信息
public:
    int from,to;
    int weight;
    friend bool operator<(Edge a,Edge b){
        if(a.weight>b.weight)
            return true;
        else
            return false;
    }
    Edge(){}
    Edge(int f,int t,int w):from(f),to(t),weight(w){}
};

vector<TreeNode>rec;//并查集父指针结点表
priority_queue<Edge>q;//存最小边的堆

int Find(int i){
    if(rec[i].father==i)
        return i;
    rec[i].father=Find(rec[i].father);
    return rec[i].father;
}

void Union(int i,int j){
    int rt1=Find(i),rt2=Find(j);
    if(rt1!=rt2){
        if(rec[rt1].weight>=rec[rt2].weight){
            rec[rt2].father=rt1;
            rec[rt1].weight+=rec[rt2].weight;
        }
        else{
            rec[rt1].father=rt2;
            rec[rt2].weight+=rec[rt2].weight;
        }
    }
}

//思路：每次取出权中最小的边，只要没有和已有的边冲突（成环，利用并查集判断）就放入MST
void Kruskal(vector<vector<int> >Graph,int n){
    rec.resize(n);
    int equNum=n;//并查集中等价类的数目
    vector<Edge>MST;
    for(int i=0;i<n;i++){
        rec[i].father=i;
        rec[i].weight=1;
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<=i;j++){//因为是无向图，要防止重复边！！！
            if(Graph[i][j]){
                q.push(Edge(i,j,Graph[i][j]));
            }
        }
    }
    while(equNum>1){//等价类大于1说明还没生成MST
        if(q.empty()){
            cout<<"There is not a MST in the graph.\n";
            return;
        }
        Edge e=q.top();
        q.pop();
        int rt1=Find(e.from),rt2=Find(e.to);
        if(rt1!=rt2){//不能成环了
            Union(rt1,rt2);
            equNum--;
            MST.push_back(e);
        }
    }
    for(int i=0;i<n-1;i++)
        cout<<MST[i].from<<' '<<MST[i].to<<' '<<MST[i].weight<<endl;
}

int main(){
    vector<vector<int> >Graph(7,vector<int>(7,0));
    Graph[0][4]=1,Graph[0][1]=20;
    Graph[1][0]=20,Graph[1][3]=4,Graph[1][2]=6;
    Graph[2][1]=6,Graph[2][6]=2;
    Graph[3][1]=4,Graph[3][6]=8,Graph[3][5]=12;
    Graph[4][0]=1,Graph[4][5]=15;
    Graph[5][3]=12,Graph[5][4]=15,Graph[5][6]=10;
    Graph[6][5]=10,Graph[6][3]=8,Graph[6][2]=2;
    Kruskal(Graph,7);
    return 0;
}