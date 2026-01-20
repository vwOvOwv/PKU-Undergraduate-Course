#include <iostream>
#include<queue>
#include<vector>
using namespace std;

#define INFINITE INT_MAX

class Dist{
public:
	int index;
	int length;
	int pre;
	friend bool operator<(Dist a,Dist b){//重载小于号，优先队列用小于号比较
		if(a.length<b.length)
			return true;
		else 
			return false;
	}
};

priority_queue<Dist,vector<Dist> >q;//堆
vector<vector<int> >Graph(5,vector<int>(5));//图

//思路：将点集合分为两部分。一部分是已经确定最短路径了的，其余的为另一部分
void Dijkstra(int n,int s){
	vector<Dist>rec(n);//已经确定最短路径的点的集合
	vector<bool>visited(n);//标记已经确定最短路径的点
	for(int i=0;i<n;i++){//初始化
		rec[i].index=i;
		rec[i].length=INFINITE;//初始化为无穷大
		rec[i].pre=s;//假定s可以到达所有点，只是距离可以是无穷大
		visited[i]=0;
	}
	// visited[s]=1; //这时还没访问呢！
	rec[s].length=0;//到自己最短距离为0，这是目前唯一确定的
	q.push(rec[s]);//初始化堆
	for(int i=0;i<n;i++){
		bool found=false;
		Dist tmp;
		while(!q.empty()){
			tmp=q.top();
			q.pop();
			if(!visited[tmp.index]){//找到了没访问过的点且当前源点到这个点的距离比到其他所有点都短
				found=true;
				break;
			}
		}
		if(!found)//已经全部访问过了，可以结束算法
			break;
		visited[tmp.index]=1;//访问该点
		for(int i=0;i<n;i++){
			if(Graph[tmp.index][i]){
				if(rec[i].length>rec[tmp.index].length+Graph[tmp.index][i]){//更新源点到所有点的最短距离（但其实已经确定了最短距离的点是不会被更新的）
					rec[i].length=rec[tmp.index].length+Graph[tmp.index][i];
					rec[i].pre=tmp.index;//注意改变前驱
					q.push(rec[i]);//重新入堆，不管访问没访问过
				}
			}
		}
	}

	cout<<"index  ";
	for(auto i:rec){
		cout<<i.index<<' ';
	}
	cout<<endl;

	cout<<"length  ";
	for(auto i:rec){
		cout<<i.length<<' ';
	}
	cout<<endl;

	cout<<"pre  ";
	for(auto i:rec){
		cout<<i.pre<<' ';
	}
	cout<<endl;
	cout<<endl;
}

int main(){
	//  {0, 10,  0, 30, 100},
    //  {0,  0, 50,  0,  0}, 
    //  {0,  0,  0,  0, 10}, 
    //  {0, 10, 20,  0, 60}, 
    //  {0,  0,  0,  0,  0} 
	Graph[0][1]=Graph[4][1]=Graph[2][4]=10;
	Graph[3][2]=20;
	Graph[0][3]=30;
	Graph[1][2]=50;
	Graph[3][4]=60;
	Graph[0][4]=100;
	
	// Graph[0][1]=10;Graph[0][2]=6;
	// Graph[1][2]=-8;
	// Graph[2][3]=Graph[3][4]=1;

	// Graph[0][1]=-1,Graph[1][2]=-2,Graph[2][0]=-3;

	//有负权值不一定总是错误，下面是错误的例子
	// Graph[0][1]=2;Graph[0][2]=5;
	// Graph[1][3]=-2;
	// Graph[2][3]=2;
	// Graph[3][4]=1;
	Dijkstra(5,0);

	for(int i=0;i<5;i++){//基于Dijkstra算法计算每对结点之间的最短路径
		Dijkstra(5,i);
	}
	return 0;
}

