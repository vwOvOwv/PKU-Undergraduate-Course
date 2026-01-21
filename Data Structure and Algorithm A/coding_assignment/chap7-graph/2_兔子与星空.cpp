#include<iostream>
#include<vector>
#include<queue>
using namespace std;

class vectorNode{
public:
    int index;
    vector<pair<int,int> >adj;
    vectorNode(){}
    vectorNode(int i):index(i){}
};

class Edge{
public:
    int from,to,weight;
    Edge(int f,int t,int w):from(f),to(t),weight(w){}
    Edge(){}
    friend bool operator<(Edge a,Edge b){
        if(a.weight<b.weight)
            return false;
        else
            return true;
    }
};

class TreeNode{
public:
    int father;
    int weight;
    TreeNode(){}
    TreeNode(int f,int w):father(f),weight(w){}
};

int Find(vector<TreeNode>&rec,int i){
    if(rec[i].father==i)
        return i;
    rec[i].father=Find(rec,rec[i].father);
    return rec[i].father;
}

void Union(vector<TreeNode>&rec,int i,int j){
    int rt1=Find(rec,i),rt2=Find(rec,j);
    if(rt1!=rt2){
        if(rec[rt1].weight>=rec[rt2].weight){
            rec[rt2].father=rt1;
            rec[rt1].weight+=rec[rt2].weight;
        }
        else{
            rec[rt1].father=rt2;
            rec[rt2].weight+=rec[rt1].weight;
        }
    }
}

void Kruskal(vector<vectorNode>Graph,int n){
    vector<TreeNode>rec(n);
    priority_queue<Edge>q;
    int ans=0,equNum=n;
    for(int i=0;i<n;i++){
        rec[i].father=i;
        rec[i].weight=1;
    }
    for(int i=0;i<n;i++){
        int size=Graph[i].adj.size();
        for(int j=0;j<size;j++){
            q.push(Edge(i,Graph[i].adj[j].first,Graph[i].adj[j].second));
        }
    }
    while(equNum>1){
        Edge e=q.top();
        q.pop();
        int rt1=Find(rec,e.from),rt2=Find(rec,e.to);
        if(rt1!=rt2){
            Union(rec,rt1,rt2);
            equNum--;
            ans+=e.weight;
        }
    }
    cout<<ans<<endl;
}

int main(){
    int n;
    cin>>n;
    vector<vectorNode>Graph(n);
    char ch;int adjNum,weight;
    for(int i=0;i<n-1;i++){
        cin>>ch;
        int cur=ch-'A';
        cin>>adjNum;
        for(int j=0;j<adjNum;j++){
            cin>>ch>>weight;
            Graph[cur].adj.push_back(make_pair(ch-'A',weight));
            //Graph[ch-'A'].adj.push_back(make_pair(cur,weight));
        }
    }
    Kruskal(Graph,n);
    return 0;
}