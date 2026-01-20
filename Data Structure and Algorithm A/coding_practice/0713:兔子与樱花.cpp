#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<queue>
#include<climits>

using namespace std;

class Dist{
public:
    int index,length,pre;
    Dist(){}
    Dist(int i,int l,int p):index(i),length(l),pre(p){}
    friend bool operator<(Dist a,Dist b){
        return a.length>b.length;
    }
};

unordered_map<string,int>mem;
vector<string>rec;

void Dijkstra(vector<vector<int> >Graph,int n,string s1,string s2){
    priority_queue<Dist>q;
    vector<Dist>dis(n);
    vector<int>visited(n,0);
    int s=mem[s1];
    for(int i=0;i<n;i++){
        dis[i].index=i;
        dis[i].pre=s;
        dis[i].length=INT_MAX;
    }
    dis[s].pre=-1;
    dis[s].length=0;
    q.push(dis[s]);
    for(int k=0;k<n;k++){
        bool found=false;
        Dist p;
        while(!q.empty()){
            p=q.top();
            q.pop();
            if(!visited[p.index]){
                found=true;
                break;
            }
        }
        if(!found){
            goto loop;
        }
        visited[p.index]=1;
        for(int i=0;i<n;i++){
            if(Graph[p.index][i]){
                if(dis[i].length>dis[p.index].length+Graph[p.index][i]){
                    dis[i].length=dis[p.index].length+Graph[p.index][i];
                    dis[i].pre=p.index;
                    q.push(dis[i]);
                }
            }
        }
    }
    loop:;
    int des=mem[s2];
    int tmp=des;
    vector<Dist>path;
    while(dis[tmp].pre!=-1){
        path.push_back(dis[tmp]);
        tmp=dis[tmp].pre;
    }
    path.push_back(dis[tmp]);
    int size=path.size();
    for(int i=size-1;i>=0;i--){
        if(i!=size-1)
            cout<<"->"<<'('<<Graph[path[i].pre][path[i].index]<<')'<<"->";
        cout<<rec[path[i].index];
    }
    cout<<endl;
}

int main(){
    int p;
    cin>>p;
    rec.resize(p);
    for(int i=0;i<p;i++){
        cin>>rec[i];
        mem[rec[i]]=i;
    }
    int q;
    vector<vector<int> >Graph(p,vector<int>(p,0));
    cin>>q;
    string s1,s2;
    int dis;
    for(int i=0;i<q;i++){
        cin>>s1>>s2>>dis;
        int place1=mem[s1],place2=mem[s2];
        Graph[place1][place2]=Graph[place2][place1]=dis;
    }
    int r;
    cin>>r;
    for(int i=0;i<r;i++){
        cin>>s1>>s2;
        Dijkstra(Graph,p,s1,s2);
    }
}