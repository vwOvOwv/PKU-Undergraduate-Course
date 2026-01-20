#include<iostream>
#include<vector>

using namespace std;

class Node{
public:
    int x,y,prev;
    bool gotKey;
    Node(){}
    Node(int xx,int yy,int pp,bool g):x(xx),y(yy),prev(pp),gotKey(g){}
};

int m,n;
int sx,sy,ex,ey;
char rec[110][110];
int visited[110][110][2];

int dx[4]={-1,1,0,0},dy[4]={0,0,-1,1};

vector<Node>q(10010);

int head=0,tail=0;

void push(Node a){
    q[tail]=a;
    tail++;
}

void pop(){
    head++;
}

Node&front(){
    return q[head];
}

bool empty(){
    return head==tail;
}

void bfs(){
    visited[sx][sy][0]=1;
    push(Node(sx,sy,-1,false));
    while(!empty()){
        Node p=front();
        if(p.x==ex&&p.y==ey&&p.gotKey){
            vector<pair<int,int> >path;
            Node tmp=p;
            while(tmp.prev!=-1){
                path.push_back(make_pair(tmp.x,tmp.y));
                tmp=q[tmp.prev];
            }
            path.push_back(make_pair(tmp.x,tmp.y));
            int size=path.size();
            for(int i=size-1;i>=0;i--){
                cout<<path[i].first<<' '<<path[i].second<<endl;
            }
            return;
        }
        if(rec[p.x][p.y]=='Y'){
            // cout<<"called"<<endl;
            rec[p.x][p.y]='0';
            p.gotKey=true;
        }
        for(int i=0;i<4;i++){
            int x=p.x+dx[i],y=p.y+dy[i];
            if(x>=1&&x<=m&&y>=1&&y<=n&&!visited[x][y][p.gotKey]&&rec[x][y]!='1'){
                visited[x][y][p.gotKey]=1;
                push(Node(x,y,head,p.gotKey));
            }
        }
        pop();
    }
}

int main(){
    cin>>m>>n;
    for(int i=1;i<=m;i++){
        for(int j=1;j<=n;j++){
            cin>>rec[i][j];
            if(rec[i][j]=='R'){
                sx=i,sy=j;
            }
            else if(rec[i][j]=='C'){
                ex=i,ey=j;
            }
        }
    }
    bfs();
}