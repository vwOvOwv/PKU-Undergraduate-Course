// 图用相邻矩阵表示方法

//*****************************************************
//这里要注意 图类中的边为无穷大的时候也应该看成是一条边
//所以要修改一下IsEdge函数的定义
//******************************************************


#include <iostream>
#include <queue>
using namespace std;
#define N 3 // 定义图的顶点数
#define INFINITE 0xffff

#include "Graphm.h"


class Dist  {      //定义Dist类，下面的Dijkstra算法和Floyd算法要用到
public:
	int index;      //顶点的索引值，仅Dijkstra算法会用到
	int length;     //顶点之间的距离
	int pre;       //路径最后经过的顶点
	Dist() {};
	~Dist() {};

	bool operator < (const Dist & arg)  {
		return (length < arg.length);
	}
	bool operator == (const Dist &arg)  {
		return (length==arg.length);
	}
	bool operator > (const Dist &arg)  {
		return (length>arg.length);
	}
	bool operator <=(const Dist &arg)  {
		return (length<=arg.length);
	}
	bool operator >= (const Dist &arg)  {
		return (length>=arg.length);
	}
};


//[算法7.9] Floyd算法
void Floyd(Graph& G, Dist** &D)  {
	int i,j,v;                               // i,j,v是计数器
	D = new Dist*[G.VerticesNum()];          // 为数组D申请空间
    for (i = 0; i < G.VerticesNum();i++) 
		D[i] = new Dist[G.VerticesNum()];
	for (i = 0;i < G.VerticesNum();i++)          // 初始化数组D
		for (j = 0;j < G.VerticesNum();j++) {
			if (i == j) {
				D[i][j].length = 0;
				D[i][j].pre = i;
			}
			else {
				D[i][j].length = INFINITE;
				D[i][j].pre = -1;
			}
		}
		for (v = 0;v < G.VerticesNum();v++)
			for (Edge e = G.FirstEdge(v); G.IsEdge(e);e = G.NextEdge(e)) {
				D[v][G.ToVertex(e)].length = G.Weight(e);		
				D[v][G.ToVertex(e)].pre = v;	
			}
			// 如果两个顶点间的最短路径经过顶点v，则更新最短距离
			for (v = 0;v < G.VerticesNum();v++)
				for (i = 0;i < G.VerticesNum();i++)
					for (j = 0;j < G.VerticesNum();j++)
						if (D[i][j].length > (D[i][v].length+D[v][j].length)) {
							D[i][j].length = D[i][v].length+D[v][j].length;
							D[i][j].pre = D[v][j].pre;
						}
}



int A[N][N] =  {
//	V0    V1    V2
	0,    4,    11,
	6,    0,     2,
    3, INFINITE, 0
};



void main()  {
	Graphm aGraphm(N); // 建立图
	aGraphm.IniGraphm(&aGraphm, A); // 初始化图

	Dist **D;
	Floyd(aGraphm, D);
	
	for (int i = 0; i < N; i ++)  {
		for (int j = 0; j < N; j ++)
			cout << D[i][j].length << " ";
		cout << endl;
	}
}
