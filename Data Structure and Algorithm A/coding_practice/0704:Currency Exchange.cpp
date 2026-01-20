#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<queue>

using namespace std;

class Edge{
public:
    int to;
    double rate,commission;
    Edge(){}
    Edge(int t=0,double r=0,double c=0):
        to(t),rate(r),commission(c){}
};

int n,m,s;
double start;

void SPFA(vector<vector<Edge> >rec,int n){
    vector<int>inQueue(n+1,0);
    vector<double>dis(n+1,-1);
    queue<int>q;
    q.push(s);
    dis[s]=start;
    inQueue[s]=1;
    while(!q.empty()){
        // for(int i=1;i<=n;i++)
        //     cout<<dis[i]<<' ';
        // cout<<endl;
        int p=q.front();
        q.pop();
        inQueue[p]=0;
        int size=rec[p].size();
        // cout<<"size="<<size<<endl;
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
    cin>>n>>m>>s>>start;
    vector<vector<Edge> >rec(n+1);
    for(int i=0;i<m;i++){
        int A,B;
        double rAB,cAB,rBA,cBA;
        cin>>A>>B>>rAB>>cAB>>rBA>>cBA;
        rec[A].push_back(Edge(B,rAB,cAB));
        rec[B].push_back(Edge(A,rBA,cBA));
    }
    SPFA(rec,n);
    return 0;
}