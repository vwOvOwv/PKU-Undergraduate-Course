//***********   Graph.h   ************//
//图的基类

#define UNVISITED 0
#define VISITED 1

//Edge类
class Edge  { 
public: 
	int weight;    //weight是边的权
	int from;      //from是边的始点
	int to;        //to是边的终点
	Edge()  {		// 构造函数
		from = -1;  to = -1;  weight = 0;
	}							
	Edge(int f,int t,int w)   { // 构造函数
		from=f;   to=t;   weight=w;
	}
	bool operator < (const Edge &arg) {return (this->weight < arg.weight);};
	bool operator == (const Edge &arg) {return (this->weight == arg.weight);};
	bool operator > (const Edge &arg) {return (this->weight > arg.weight);};
	bool operator <= (const Edge &arg) {return (this->weight <= arg.weight);};
	bool operator >= (const Edge &arg) {return (this->weight >= arg.weight);};
};

//图基类
class Graph  {
public:
	int numVertex;             //图的顶点的个数
	int numEdge;				//图的边的数目
	int *Mark;					/*Mark指针指向保存有图的顶点的标志位的数组,标志位用来标记某顶点是否被访问过*/
	int *Indegree;				//Indegree指针指向保存有图的顶点的入度的数组
	Graph(int numVert)  {		//构造函数
		numVertex = numVert;      //确定图的顶点的个数
		numEdge = 0;				//确定图的边的数目
		Indegree = new int[numVertex]; /*为保存图的顶点的入度申请数组,Indegree为数组指针*/
		Mark = new int[numVertex];	 /*为图的顶点的标志位申请数组,Mark为数组指针*/
		
		for (int i = 0; i < numVertex; i ++)  {	/*确定图的顶点的标志位和入度,即所有顶点的标志位初始化为未被访问过,入度初始化为0*/
			Mark[i] = UNVISITED;
			Indegree[i] = 0;
		}
	}
	
	~Graph() {				//析构函数
		delete [] Mark;
		delete [] Indegree;
	}
	
	virtual Edge FirstEdge(int oneVertex) {	// 返回与顶点oneVertex相关联的第一条边
		Edge myEdge;
		myEdge.from = oneVertex; 
		myEdge.to = -1;
		return myEdge;
	}
	
	virtual Edge NextEdge(Edge preEdge) {      // 返回与边PreEdge有相同关联顶点的下一条边
		return preEdge;
	}

	int VerticesNum() {		//返回图的顶点个数
		return numVertex;
	}
	
	int EdgesNum() {			//返回图的边数
		return numEdge;
	}
	
	int FromVertex(Edge oneEdge) {  // 返回oneEdge的始点
		return oneEdge.from;
	}
	
	int ToVertex(Edge oneEdge) {	// 返回oneEdge的终点
		return oneEdge.to;
	}
	
	int Weight(Edge oneEdge) {		// 返回oneEdge的权值
		return oneEdge.weight;
	}
	
	bool IsEdge(Edge oneEdge)  {	//如果oneEdge是边则返回TRUE，否则返回FALSE
		if (oneEdge.weight > 0 && oneEdge.weight <= INFINITE && oneEdge.to >= 0)
			return true;
		else
			return false;
	}
	
	virtual void setEdge(int from, int to, int weight) = 0;
	virtual void delEdge(int from, int to) = 0;
};
