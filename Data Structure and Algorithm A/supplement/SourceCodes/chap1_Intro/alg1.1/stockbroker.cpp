#include <iostream>
#include <string>

using namespace std;

#define MAX 102
#define INFINITY 65536

#include "Floyd.h" 

//void Floyd(Graph& G, Dist** &D); 

int BrokerToPassRomour (Graph& G )  {	
	Dist **D ;							// 传入Floyd算法的参数，求出各顶点间的最短距离
	int* max = new int[G.VerticesNum()];				// 记录每个顶点到其他顶点最短距离的最大值
	int min = 0;						// 记录max数组中的最小值
	int pos = 0;// 记录max数组中最小值的位置
	int i;                                
	for( int i = 0; i < G.VerticesNum(); i++ )   	// 初始化max数组
		max[i] = -1 ;		

	Floyd(G,  D);						// 调用Floyd算法求每个结点之间的最短距离

	for (i = 0; i < G.VerticesNum();i ++ )  {  // 找出每行的最大值
		for (int j = 0; j < G. VerticesNum(); j ++ )  {
			if ( D[i][j].length > max[i] )
				max[i] = D[i][j].length ;
		}
	}

	min = max[0];
	for ( i = 1 ; i < G.VerticesNum(); i++ )		    // 找max数组的最小值
		if ( max[i] < min )  {
			min = max[i];
			pos = i;							
		}
		if (min == INFINITY)  {
			cout<<"此图不连通"<<endl;
			return -1;
		}
		return pos;								// 返回最合适的开始顶点
}

