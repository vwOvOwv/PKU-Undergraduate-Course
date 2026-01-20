#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<queue>

using namespace std;

class Edge{
public:
    int from,to,weight;
    Edge(){}
    Edge(int f,int t,int w):from(f),to(t),weight(w){}
    friend bool operator<(Edge a,Edge b){
        return a.weight>b.weight;
    }
};

class TreeNode{
public:
    int father,weight;
    TreeNode(){}
    TreeNode(int f,int w):father(f),weight(w){}
};
vector<TreeNode>rec;
int Find(int i){
    if(rec[i].father==i)
        return i;
    rec[i].father=Find(rec[i].father);
    return rec[i].father;
}
void Union(int i,int j){
    int rt1=Find(i),rt2=Find(j);
    if(rt1!=rt2){
        if(rec[rt1].weight<rec[rt2].weight){
            rec[rt2].weight+=rec[rt1].weight;
            rec[rt1].father=rt2;
        }
        else{
            rec[rt2].father=rt1;
            rec[rt1].weight+=rec[rt2].weight;
        }
    }
}

void Kruskal(vector<vector<int> >Graph,int n){
    priority_queue<Edge>q;
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            if(Graph[i][j]){
                q.push(Edge(i,j,Graph[i][j]));
            }
        }
    }
    rec.resize(n);
    for(int i=0;i<n;i++){
        rec[i].father=i;
        rec[i].weight=1;
    }
    int cnt=0;
    int equNum=n;
    while(equNum>1){
        Edge p=q.top();
        q.pop();
        if(Find(p.from)!=Find(p.to)){
            cnt+=p.weight;
            Union(p.from,p.to);
            equNum--;
        }
    }
    cout<<cnt<<endl;
}

int main(){
    int n;
    cin>>n;
    vector<vector<int> >Graph(n+1,vector<int>(n+1,0));
    for(int i=0;i<n-1;i++){
        char from;
        int m;
        cin>>from>>m;
        for(int i=0;i<m;i++){
            char to;int w;
            cin>>to>>w;
            Graph[from-'A'][to-'A']=w;
            Graph[to-'A'][from-'A']=w;
        }
    }
    Kruskal(Graph,n);
}