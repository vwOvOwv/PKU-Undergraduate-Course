#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<set>
#include<queue>

using namespace std;

long long int rec[510][510];
long long int maxi=0;
set<pair<int,int> >mem;
int m,n;
bool con=false;
int cnt=0;
int dx[4]={-1,1,0,0},dy[4]={0,0,-1,1};

class Node{
public:
    int x,y;
    Node(){}
    Node(int xx,int yy):x(xx),y(yy){}
};

void bfs(int sx,int sy,long long int d){
    int tmp=cnt;
    vector<vector<int> >visited(m,vector<int>(n,0));
    queue<Node>q;
    q.push(Node(sx,sy));
    visited[sx][sy]=1;
    while(!q.empty()){
        Node p=q.front();
        q.pop();
        if(mem.count(make_pair(p.x,p.y))){
            tmp--;
            if(tmp==0){
                con=true;
                return;
            }
        }
        for(int i=0;i<4;i++){
            int x=p.x+dx[i],y=p.y+dy[i];
            if(x>=0&&x<m&&y>=0&&y<n&&!visited[x][y]){
                if(abs(rec[p.x][p.y]-rec[x][y])<=d){
                    visited[x][y]=1;
                    q.push(Node(x,y));
                }
            }
        }
    }
}

int main(){
    cin>>m>>n;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            cin>>rec[i][j];
            maxi=max(maxi,rec[i][j]);
        }
    }
    // cout<<"called1"<<endl;//
    int num;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            cin>>num;
            if(num){
                mem.insert(make_pair(i,j));
                cnt++;
            }
        }
    }
    long long int left=0,right=maxi;
    // cout<<"cnt="<<cnt<<endl;//
    // cout<<"maxi="<<maxi<<endl;//
    long long int d;
    int sx=(*mem.begin()).first,sy=(*mem.begin()).second;
    // cout<<sx<<' '<<sy<<endl;//
    while(left<=right){
        long long int mid=left+(right-left)/2;
        con=false;
        bfs(sx,sy,mid);
        if(con){
            d=mid;
            right=mid-1;
        }
        else{
            left=mid+1;
        }
    }
    cout<<d<<endl;
}