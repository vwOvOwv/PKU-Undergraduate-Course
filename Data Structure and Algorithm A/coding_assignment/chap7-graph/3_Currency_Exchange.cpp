#include<iostream>
#include<vector>
#include<queue>

using namespace std;

class Edge{
public:
    int to;
    double rate;
    double commission;
    Edge(){}
    Edge(int t,double r,double c):to(t),rate(r),commission(c){}
};

int n,m,type;
double start;

void SPFA(vector<vector<Edge> >rec,int n,int s,double start){
    vector<int>inQueue(n+1,0);
    queue<int>q;
    vector<double>dis(n+1,-1);
    dis[s]=start;
    q.push(s);
    inQueue[s]=1;
    while(!q.empty()){
        int p=q.front();
        q.pop();
        inQueue[p]=0;
        int size=rec[p].size();
        for(int i=0;i<size;i++){
            int des=rec[p][i].to;
            if(dis[des]<(dis[p]-rec[p][i].commission)*rec[p][i].rate){
                dis[des]=(dis[p]-rec[p][i].commission)*rec[p][i].rate;
                if(!inQueue[des]){
                    q.push(des);
                    inQueue[des]=1;
                }
            }
        }
        if(dis[s]>start){
            cout<<"YES"<<endl;
            return;
        }
    }
    cout<<"NO"<<endl;
}

int main(){
    cin>>n>>m>>type>>start;
    vector<vector<Edge> >rec(n+1);
    for(int i=0;i<m;i++){
        int A,B;
        double rAB,cAB,rBA,cBA;
        cin>>A>>B>>rAB>>cAB>>rBA>>cBA;
        rec[A].push_back(Edge(B,rAB,cAB));
        rec[B].push_back(Edge(A,rBA,cBA));
    }
    SPFA(rec,n,type,start);
    return 0;
}