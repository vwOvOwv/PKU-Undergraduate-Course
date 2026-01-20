// 图的相邻矩阵表示存储图，实现图的拓扑排序
#include <iostream>
#include <queue>
#include "Graphm.h"
using namespace std;

// 函数功能：显示排序后的序列
void Visit(Graph &G, int v) {
	cout << "C" << v << " ";
}
void Visit(int v) {
	cout << "C" << v << " ";
}

//***************************************************************
//[算法7.7] 队列实现的图拓扑排序
void TopsortbyQueue(Graph& G)  {      //队列方式实现的拓扑排序
	for(int i=0;i<G.VerticesNum();i++)    //初始化Mark数组
		G.Mark[i]=UNVISITED;
    using std::queue;
	queue<int> Q; //初始化队列
	int i;
	for(i=0; i<G.VerticesNum(); i++)
		if(G.Indegree[i]==0)
			Q.push(i);                //图中入度为0的顶点入队
		while(!Q.empty())  {              //如果队列中还有图的顶点
			int V=Q.front();
			Q.pop();                     //一个顶点出队
			Visit(G,V);
			G.Mark[V]=VISITED;
			for(Edge e= G.FirstEdge(V);G.IsEdge(e);e=G.NextEdge(e))  {
				G.Indegree[G.ToVertex(e)]--;  //所有与之相邻的顶点入度-1
				if(G.Indegree[G.ToVertex(e)]==0)
					Q.push(G.ToVertex(e));   //入度为0的顶点入队
			}
		}
		for(i=0; i<G.VerticesNum(); i++)
			if(G.Mark[i]==UNVISITED)  {
				cout<<" 此图有环！";        //图有环
				break;
			}	
}


int A[N][N] =  {
      //C0  C1  C2  C3  C4  C5  C6  C7  C8	
	    0,  0,  1,  0,  0,  0,  0,  1,  0, 
		0,  0,  1,  1,  1,  0,  0,  0,  0,   
		0,  0,  0,  1,  0,  0,  0,  0,  0,  
		0,  0,  0,  0,  0,  1,  1,  0,  0,  
		0,  0,  0,  0,  0,  1,  0,  0,  0,  
		0,  0,  0,  0,  0,  0,  0,  0,  0,  
		0,  0,  0,  0,  0,  0,  0,  0,  0,  
		0,  0,  0,  0,  0,  0,  0,  0,  1,  
		0,  0,  0,  0,  0,  0,  1,  0,  0};  //该图为图7.18表示课程优先关系的有向无环图

int main()
{
	Graphm aGraphm(N);              // 建立图
	aGraphm.IniGraphm(&aGraphm, A); // 初始化图
	cout << "Top sort by Queue is : ";
	TopsortbyQueue(aGraphm);
	cout << "\nOK! " << endl; 
	return 0;
}