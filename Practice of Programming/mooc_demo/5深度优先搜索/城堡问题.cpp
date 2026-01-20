/*
解题思路：
把方块看作节点，相邻两个方块之间如果没有墙则在方块之间连一条边，这样城堡就能转换成一个图
求房间个数即为求图中极大连通子图的数目
对每一个房间进行深度优先搜索，给这个房间能够到达的所有位置"染色"
最后统计用了几种颜色以及每种颜色的数量
*/
#include<iostream>
#include<stack>
#include<cstring>
using namespace std;
int R,C;
int rooms[60][60];//图的信息已经存在了rooms里
int color[60][60];//房间染色的标记
int maxRoomArea=0,roomNum=0;
int roomArea;
void dfs(int i,int k){
	if(color[i][k]!=0)
		return;
	roomArea++;
	color[i][k]=roomNum;//做标记
	//利用二进制
	if((rooms[i][k]&1)==0)//向西走
		dfs(i,k-1);
	if((rooms[i][k]&2)==0)//向北走
		dfs(i-1,k);
	if((rooms[i][k]&4)==0)//向东走
		dfs(i,k+1);
	if((rooms[i][k]&8)==0)//向南走
		dfs(i+1,k);
}
int main()
{
	cin>>R>>C;
	for(int i=1;i<=R;i++)
		for(int k=1;k<=C;k++)
			cin>>rooms[i][k];
	memset(color,0,sizeof(color));
	for(int i=1;i<=R;i++)
		for(int k=1;k<=C;k++){//遍历每一个房间
			if(color[i][k]==0){
				roomNum++;
				roomArea=0;//注意要清零
				dfs(i,k);
				maxRoomArea=max(roomArea,maxRoomArea);
			}
		}
	cout<<roomNum<<endl;
	cout<<maxRoomArea<<endl;
}
