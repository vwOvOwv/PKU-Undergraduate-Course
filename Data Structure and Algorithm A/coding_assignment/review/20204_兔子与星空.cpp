#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<queue>

using namespace std;

class Edge{
public:
    int from,to;
    int weight;
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

priority_queue<Edge>q;
vector<TreeNode>v;

int Find(int i){
    if(v[i].father==i)
        return i;
    v[i].father=Find(v[i].father);
    return v[i].father;
}

void Union(int i,int j){
    int rt1=Find(i),rt2=Find(j);
    if(rt1!=rt2){
        if(v[rt1].weight<v[rt2].weight){
            v[rt2].weight+=v[rt1].weight;
            v[rt1].father=rt2;
        }
        else{
            v[rt1].weight+=v[rt2].weight;
            v[rt2].father=rt1;
        }
    }
}

void Kruskal(int n){
    v.resize(n);
    for(int i=0;i<n;i++){
        v[i].weight=1;
        v[i].father=i;
    }
    int equNum=n;
    int ans=0;
    while(equNum>1){
        // for(int i=0;i<n;i++){
        //     cout<<v[i].father<<' ';
        // }
        // cout<<endl;
        Edge p=q.top();
        // cout<<p.weight<<endl;//
        q.pop();
        int from=p.from;
        int to=p.to;
        if(Find(from)!=Find(to)){
            Union(from,to);
            // cout<<p.weight<<endl;
            ans+=p.weight;
            equNum--;
        }
    }
    cout<<ans<<endl;
}

int main(){
    int n;
    cin>>n;
    // vector<vector<Edge> >rec(n);
    for(int i=0;i<n-1;i++){
        char from,to;
        int m,w;
        cin>>from>>m;
        for(int j=0;j<m;j++){
            cin>>to>>w;
            Edge tmp=Edge(from-'A',to-'A',w);
            // rec[from-'A'].push_back(tmp);
            q.push(tmp);
        }
    }
    // cout<<q.size()<<endl;
    // cout<<"called"<<endl;
    Kruskal(n);
}