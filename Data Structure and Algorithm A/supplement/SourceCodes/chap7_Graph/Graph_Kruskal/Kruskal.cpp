#include <iostream>
#include <queue>
#define UNVISITED 0
#define VISITED 1
#define INFINITE 9999
#define N 7         // 定义图的顶点数
using namespace std;
#include "Graphm.h"
#include "MinHeap.h"
#include "ParTree.h"

//最小支撑树的Kruskal算法，
//参数G是图，参数MST是保存最小支撑树中所有边的数组
void AddEdgetoMST(Edge e, Edge *&MST, int n)
{
	MST[n] = e;
}

void Kruskal(Graph& G, Edge* &MST)  {
	ParTree<int> A(G.VerticesNum());           //等价类
    MinHeap<Edge> H(G.EdgesNum());        //最小值堆（minheap）    
	MST=new Edge[G.VerticesNum()-1];      //最小支撑树
	int MSTtag=0;                         //最小支撑树边的标号
    for(int i=0; i<G.VerticesNum(); i++)  //将图的所有边插入最小值堆H中
		for(Edge e= G. FirstEdge(i); G.IsEdge(e);e=G. NextEdge(e))
			if(G.FromVertex(e)< G.ToVertex(e))  //因为是无向图，所以应防止重复插入
				H.Insert(e);
	int EquNum=G.VerticesNum();              //开始时有|V|个等价类
    while(EquNum>1)  {                     //合并等价类
		Edge e=H.RemoveMin();               //获得下一条权最小的边
		if(e.weight==INFINITE)  {
			cout << "不存在最小支撑树." <<endl;
            delete [] MST;                     //释放空间
            MST=NULL;                   //MST是空数组
			return;
		}
        int from=G.FromVertex(e);            //记录该条边的信息
        int to= G.ToVertex(e);
        if(A.Different(from,to))  {            //如果边e的两个顶点不在一个等价类
			A.Union(from,to);     //将边e的两个顶点所在的两个等价类合并为一个
			AddEdgetoMST(e,MST,MSTtag++); //将边e加到MST
			EquNum--;                     //将等价类的个数减1
		}
	}
}


int A[N][N] =  {
	//   v0  v1  v2  v3  v4  v5  v6     
		0, 20,  0,  0,  1,  0,  0,
		20,  0,  6,  4,  0,  0,  0,
		0,  6,  0,  0,  0,  0,  2,
		0,  4,  0,  0,  0, 12,  8,
		1,  0,  0,  0,  0, 15,  0,
		0,  0,  0, 12, 15,  0, 10,
		0,  0,  2,  8,  0, 10,  0,
};			                    	//图7.24 带权图

int main()
{
 Graphm aGraphm(N); // 建立图
 aGraphm.IniGraphm(&aGraphm, A); // 初始化图
 
 Edge *D;
 Kruskal(aGraphm, D); for (int i = 0; i < N - 1; i ++)
  cout << "V" << D[i].from << "->V" << D[i].to << "   Weight is : " << D[i].weight << endl;
  return 0;
}