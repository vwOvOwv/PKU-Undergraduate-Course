//GraphMatrix.h

//Edge
class Edge
{ 
public: 
	int weight;  // Edge weight
	int from; 
	int to; // form To to 
	Edge(){};								// 构造函数
	Edge(int f,int t,int w)   { 			// 构造函数
		from=f;   to=t;   weight=w;
	}
	bool operator < (const Edge &arg) {return (this->weight < arg.weight);};
	bool operator == (const Edge &arg) {return (this->weight == arg.weight);};
	bool operator > (const Edge &arg) {return (this->weight > arg.weight);};
	bool operator <= (const Edge &arg) {return (this->weight <= arg.weight);};
	bool operator >= (const Edge &arg) {return (this->weight >= arg.weight);};
};

// 图基类
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
	};
	~Graph() {				//析构函数
		delete [] Mark;
		delete [] Indegree;
	}; 
	virtual Edge FirstEdge(int oneVertex) {	// 返回与顶点oneVertex相关联的第一条边
		Edge myEdge;
		myEdge.from = oneVertex; 
		myEdge.to = -1;
		return myEdge;
	}; 
	virtual Edge NextEdge(Edge preEdge) {      // 返回与边PreEdge有相同关联顶点的下一条边
		return preEdge;
	};
	int VerticesNum() {		//返回图的顶点个数
		return numVertex;
	};       
	int EdgesNum() {			//返回图的边数
		return numEdge;
	};
	int FromVertex(Edge oneEdge) {  // 返回oneEdge的始点
		return oneEdge.from;
	}; 
	int ToVertex(Edge oneEdge) {	// 返回oneEdge的终点
		return oneEdge.to;
	};    
	int Weight(Edge oneEdge) {		// 返回oneEdge的权值
		return oneEdge.weight;
	}; 
	bool IsEdge(Edge oneEdge)  {	//如果oneEdge是边则返回TRUE，否则返回FALSE
		if (oneEdge.weight > 0 && oneEdge.weight < INFINITE && oneEdge.to >= 0)
			return true;
		else
			return false;
	}; 
	virtual void setEdge(int from, int to, int weight) = 0;
	virtual void delEdge(int from, int to) = 0;
};


// 图的相邻矩阵表示法
class Graphm:public Graph  {
private:
	int **matrix;				//指向相邻矩阵的指针
public:
	void IniGraphm(Graphm *Graphm, int A[N][N]); // 初始化
	void DFS(Graph &G, int V);			// 深度优先搜索  
	void BFS(Graph &G, int V);			// 广度优先搜索
	void Visit(Graph &G, int V);		// 访问
	
public:
	Graphm(int numVert):Graph(numVert)  {//构造函数
		int i, j;			//i, j作为for循环中的计数器
		matrix = (int **)new int*[numVertex]; /*申请matrix数组,该数组有numVertex个元素,每个元素是整型指针类型*/
		
		for (i = 0; i < numVertex; i ++)		/*matrix数组的每个元素,都指向一个具有numVertex个元素的数组*/
			matrix[i] = new int[numVertex];  
		for (i = 0; i < numVertex; i++)       /*相邻矩阵的所有元素都初始化为0,如果矩阵元素matrix[i][j]不为0,则表明顶点i与顶点j之间有一条边,边的权即为matrix[i][j]的值*/
			for (j = 0; j < numVertex; j ++)
				matrix[i][j] = 0;
	};
	~Graphm() {							//析构函数
		for (int i = 0; i < numVertex; i ++)
			delete [] matrix[i];			//释放每个matrix[i]申请的空间
		delete [] matrix;				//释放matrix指针指向的空间
	}; 
	Edge FirstEdge(int oneVertex)  {	//返回顶点oneVertex的第一条边
		Edge myEdge;						//边myEdge将作为函数的返回值
		myEdge.from = oneVertex;			//将顶点oneVertex作为边myEdge的始点
		//  myEdge.to = -1; 
		for (int i = 0; i < numVertex; i ++)   {/* 下面寻找第一个使得matrix[oneVertex][i]不为0的i值，
												那么边(oneVertex,i)或者弧<oneVertex,i>即为顶点oneVertex
												的第一条边，将顶点i作为边myEdge的终点边myEdge的权为
			矩阵元素matrix[oneVertex][i]的值*/
			if (matrix[oneVertex][i] != 0)
			{									
				myEdge.to = i;
				myEdge.weight = matrix[oneVertex][i];
				break;
			}
		}
		return myEdge;/*如果找到了顶点oneVertex的第一条边,则返回的myEdge确实是一条边；如果没有找到顶点oneVertex的第一条边,则myEdge的成员变量to为-1，根据IsEdge函数判断可知myEdge不是一条边*/
	}; 
	Edge NextEdge(Edge preEdge)  { //返回与边PreEdge有相同关联顶点的下一条边
		Edge myEdge;
		myEdge.from=preEdge.from; /*将边myEdge的始点置为与上一条边preEdge的始点相同*/
		if(preEdge.to<numVertex)  {
			//如果preEdge.to+1>=numVertex,那么就不存在下一条边了
			for(int i=preEdge.to+1;i<numVertex;i++)  {
				/*寻找下一个使得//matrix[preEdge.from][i]不为0的i值,那么(preEdge.from,i)或者<preEdge.from,i>即为顶点preEdge.from的下一条边*/
				if(matrix[preEdge.from][i]!=0)  {
					myEdge.to=i;
					myEdge.weight=matrix[preEdge.from][i];
					break;
				}
			}
		}
		return myEdge; /*如果找到了顶点preEdge.from的下一条边，则返回的myEdge确实是一条边；如果没有找到顶点preEdge.from的下一条边，则myEdge的成员变量to为-1，
		根据IsEdge函数判断可知myEdge不是一条边*/
	}
	
	void setEdge(int from, int to, int weight)  {	//为图设定一条边
		if (matrix[from][to] <= 0) {    //如果matrix[from][to]<=0，则边(from,to)或者<from,to>将是新增的一条边，否则该边已经存在（现在只是对该边进行修改）
			numEdge ++;
			Indegree[to] ++;
		}
		matrix[from][to] = weight;
	}; 
	
	void delEdge(int from,int to)  {     //删除图的一条边
		if(matrix[from][to]>0)  {
			/*如果matrix[from][to]>0，则边(from,to)或者<from,to>确实存在，否则该边实际上并不存在（从而不必对图的边数和顶点to的入度进行修改）*/
			numEdge--;
			Indegree[to]--;
		}
		matrix[from][to]=0;		     
	}
};

// 函数功能：显示顶点
void Graphm::Visit(Graph &G, int v) {
	cout << 'V' << v <<" ";
}


// 函数功能：初始化图
void Graphm::IniGraphm(Graphm *Graphm, int A[N][N])  {
	for (int i = 0; i < N; i ++)  {
		for (int j = 0; j < N; j ++)  {
			if (A[i][j] > 0)
				Graphm->setEdge(i, j, A[i][j]);
		}
	}
}
