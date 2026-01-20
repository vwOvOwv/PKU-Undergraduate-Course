#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int r,c;
char map[40][40];
bool visited[40][40];
vector<vector<pair<int,int> > >rec(26);

int cnt=1,maxi;
int dx[4]={0,1,0,-1},dy[4]={1,0,-1,0};
void dfs(int x,int y){
    for(int i=0;i<4;i++){
        int xx=x+dx[i],yy=y+dy[i];
        if(xx>=0&&xx<r&&yy>=0&&yy<c&&!visited[xx][yy]){
            cnt++;
            for(auto pos:rec[map[xx][yy]-'A']){
                visited[pos.first][pos.second]=1;
            } 
            dfs(xx,yy);
            for(auto pos:rec[map[xx][yy]-'A']){//回溯
                visited[pos.first][pos.second]=0;
            }
            cnt--; //回溯
        }
    }
    maxi=max(maxi,cnt);//说明走到头了
}   

int main(){
    cin>>r>>c;
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            cin>>map[i][j];
            rec[map[i][j]-'A'].push_back(make_pair(i,j));//预处理记录每个字母出现的位置，空间换时间
        }
    }
    for(auto pos:rec[map[0][0]-'A']){
        visited[pos.first][pos.second]=1;
    }
    dfs(0,0);
    cout<<maxi<<endl;
}