// ͼ�����ھ����ʾ����,��Ҫ�õ���Сֵ��
#include <iostream>
#include <queue>
#define UNVISITED 0
#define VISITED 1
#define INFINITE 9999    //�������ֵ
#define N 5 // ����ͼ�Ķ�����
using namespace std;
#include "Graph_matrix.h"
#include "MinHeap.h"


//[����7.8] Dijkstra�㷨
class Dist  {      //����Dist�࣬�����Dijkstra�㷨��Floyd�㷨Ҫ�õ�
 public:
	 int index;      //���������ֵ����Dijkstra�㷨���õ�
	 int length;     //����֮��ľ���
	 int pre;       //·����󾭹��Ķ���
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

//Dijkstra�㷨�����в���G��ͼ������s��Դ���㣬D�Ǳ�����̾��뼰��·��������
void Dijkstra(Graph& G, int s, Dist* &D)  {
	D = new Dist[G. VerticesNum()];          	// D����
	for (int i = 0; i < G.VerticesNum(); i++) {   	// ��ʼ��Mark���顢D����
		G.Mark[i] = UNVISITED;
        D[i].index = i;
        D[i].length = INFINITE;
        D[i].pre = s;
    }
    D[s].length = 0; 
    MinHeap<Dist> H(G. EdgesNum());       	// ��Сֵ�ѣ�minheap��
    H.Insert(D[s]);
	int i;
	for (i = 0; i < G.VerticesNum(); i++) {
		bool FOUND = false;
        Dist d;
        while (!H.isEmpty())  {
			d = H.RemoveMin(); 
			if(G.Mark[d.index]==UNVISITED) {                //��ӡ��·����Ϣ
				cout<< "vertex index: " <<d.index<<"   ";
				cout<< "vertex pre  : " <<d.pre  <<"   ";
				cout<< "V0 --> V" << d.index <<"  length    : " <<d.length<<endl;
			}
			
			if (G.Mark[d.index] == UNVISITED) { //�ҵ�����s����Ķ���
				FOUND = true;
				break;
			}
        }
		if (!FOUND)
            break;
        int v = d.index;
		G.Mark[v] = VISITED;           		// �Ѹõ�����ѷ�����
		// ��Ϊv�ļ��룬��Ҫˢ��v�ڽӵ��Dֵ
		for (Edge e = G.FirstEdge(v); G.IsEdge(e);e = G.NextEdge(e))
			if (D[G.ToVertex(e)].length > (D[v].length+G.Weight(e))) {
				D[G.ToVertex(e)].length = D[v].length+G.Weight(e);
				D[G.ToVertex(e)].pre = v;
				H.Insert(D[G.ToVertex(e)]);
			}
	}
}



int A[N][N] =  {          //ͼ7.20  ��Դ���·����ʾ��
//  v0  v1  v2  v3  v4  
	 0, 10,  0, 30, 100,
     0,  0, 50,  0,  0, 
     0,  0,  0,  0, 10, 
     0, 10, 20,  0, 60, 
     0,  0,  0,  0,  0, 
};

int main()
{
 Graphm aGraphm(N); // ����ͼ
 aGraphm.IniGraphm(&aGraphm, A); // ��ʼ��ͼ
 Dist *D;
 Dijkstra(aGraphm, 0, D);
 return 0;
}
