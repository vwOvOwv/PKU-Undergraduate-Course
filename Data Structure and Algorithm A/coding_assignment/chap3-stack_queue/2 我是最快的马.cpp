#include<iostream>
#include<vector>//因为要输出路径所以不能直接用队列，用数组模拟

using namespace std;

int sx,sy,ex,ey,m,cnt;
int visited[11][11];
int dx[8]={-1,-2,-2,-1,1,2,2,1},dy[8]={-2,-1,1,2,2,1,-1,-2};
class Node{
public:
    int x,y,prev,step;
    Node(int xx,int yy,int pp,int ss):x(xx),y(yy),prev(pp),step(ss){}
    Node(){}
};
vector<Node>q(200);
vector<Node>rec;
int head=0,tail=0;
bool empty(){
    return head==tail;
}
void push(Node x){
    q[tail]=x;
    tail++;
}
void pop(){
    head++;
}
Node&front(){
    return q[head];
}
bool canJump(int nowx,int nowy,int desx,int desy,int nowstep){//判断能不能跳
    if(desx<0||desx>10||desy<0||desy>10)//越界
        return false;
    if((visited[desx][desy]&&visited[desx][desy]<nowstep+1)||visited[desx][desy]==-1)//已经跳过且路径更短或有其他子在这个位置
        return false;
    int xx=desx-nowx,yy=desy-nowy;
    if(xx*xx==4){//别脚
        if(xx==2&&visited[nowx+1][nowy]==-1)
            return false;
        else if(xx==-2&&visited[nowx-1][nowy]==-1)
            return false;
    }
    if(yy*yy==4){//别脚
        if(yy==2&&visited[nowx][nowy+1]==-1)
            return false;
        else if(yy==-2&&visited[nowx][nowy-1]==-1)
            return false;
    }
    return true;
}
void bfs(){
    push(Node(sx,sy,-1,0));
    visited[sx][sy]=1;
    while(!empty()){
        Node p=front();
        //cout<<p.x<<' '<<p.y<<endl;
        if(p.x==ex&&p.y==ey){
            for(int i=head;i!=-1;i=q[i].prev){//先记录路径
                    rec.push_back(q[i]);
                }
            while(!empty()){//看后面还有没有相同的终点结点
                Node tmp=front();
                if(tmp.x==ex&&tmp.y==ey&&tmp.step==p.step)
                    cnt++;
                pop();
            }
            goto loop;
        }
        for(int i=0;i<8;i++){
            int x=p.x+dx[i],y=p.y+dy[i];
            if(canJump(p.x,p.y,x,y,p.step)){
                push(Node(x,y,head,p.step+1));
                    visited[x][y]=p.step+1;
            }
        }
        pop();
    }
    loop:;
    if(cnt==1){
        int n=rec.size();
        for(int i=n-1;i>=0;i--){
            cout<<'('<<rec[i].x<<','<<rec[i].y<<')';
            if(i>0)
                cout<<'-';
        }
        cout<<endl;
    }
    else   
        cout<<cnt<<endl;
}
int main(){
    cin>>sx>>sy>>ex>>ey>>m;
    for(int i=0;i<m;i++){
        int x,y;
        cin>>x>>y;
        visited[x][y]=-1;//要与已跳到过的标记区分开
    }
    bfs();
    return 0;
}