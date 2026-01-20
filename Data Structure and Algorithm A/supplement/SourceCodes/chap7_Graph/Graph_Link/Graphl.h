//*************** Link.h **************//

#define UNVISITED 0
#define VISITED 1
#define INFINITE 0xffffffff
#define N 5 // 定义图的顶点数
using namespace std;
struct listUnit  {    //邻接表表目中数据部分的结构定义
	int vertex;      //边的终点
	int weight;      //边的权
};

template<class Elem>   //链表元素
class Link  {
public:
	Elem element;      //表目的数据
	Link *next;        //表目指针，指向下一个表目
	Link(const Elem& elemval,Link *nextval = NULL)  { //构造函数
		element = elemval; 
		next = nextval; 
	}
	Link(Link *nextval = NULL)  { 					//构造函数
		next = nextval; 
	} 
};

template<class Elem>   //链表
class LList  {
public:
	Link<Elem>* head;  //head指针并不储存任何实际元素，其存在只是为了操作方便
	LList()  {          //构造函数
		head = new Link<Elem>();
	}
	void removeall()  {  //释放边表所有表目占据的空间
		Link<Elem> *fence;
		while(head != NULL)  { //逐步释放每个表目占据的空间
			fence = head;
			head = head->next;
			delete fence;
		}
	}
	~LList() {				//析构函数
		removeall();
	}
};


class Graphl: public Graph  {
private:
	LList<listUnit> *graList;  //graList是保存所有边表的数组	
public:
	Graphl(int numVert):Graph(numVert)  { //构造函数
		graList = new LList<listUnit>[numVertex]; /*为graList数组申请空间，图有
										  numVertex个顶点，则有numVertex个边表*/
	}

	~Graphl()  {                        //析构函数
		delete [] graList;				   //释放空间
	}
	
	Edge FirstEdge(int oneVertex)  {   //返回顶点oneVertex的第一条边
		Edge myEdge;                   //边myEdge将作为函数的返回值
		myEdge.from = oneVertex;       //将顶点oneVertex作为边myEdge的始点
									  /*graList[oneVertex].head保存的是顶点oneVertex的边表，
									  temp->next指向顶点oneVertex的第一条边(如果temp->next
									  不为null)*/
		Link<listUnit> *temp = graList[oneVertex].head;
		if(temp->next != NULL)  {        //如果顶点oneVertex的第一条边确实存在
			myEdge.to = temp->next->element.vertex;
			myEdge.weight = temp->next->element.weight;
		}
		/*如果找到了顶点oneVertex的第一条边,则返回的myEdge确实是一条边;如果没有
		找到顶点oneVertex的第一条边,则myEdge的成员变量to为-1,根据IsEdge函数判
		断可知myEdge不是一条边*/
		return myEdge;
	}	

	Edge NextEdge(Edge preEdge)  {  // 返回与边PreEdge有相同关联顶点的下一条边
		Edge myEdge;			 		// myEdge的初始成员变量to为-1
		myEdge.from = preEdge.from;  		// 将边的始点置为与上一条边的相同
		Link<listUnit> *temp = graList[preEdge.from].head;		// temp指向边表头一个
		while (temp->next != NULL && temp->next->element.vertex <= preEdge.to)
			temp = temp->next; 			// 确定边preEdge的位置
		if (temp->next != NULL) {		// 边preEdge的下一条边存在
			myEdge.to = temp->next->element.vertex;
			myEdge.weight = temp->next->element.weight;
		}
		return myEdge;					// 如果没有找到第一条边，myEdge.to=-1
	}

	void setEdge(int from,int to,int weight)  {  //为图设定一条边
		Link<listUnit> *temp = graList[from].head;  /*graList[from].head保存的是顶点from的
													边表，temp->next指向顶点from的第一条边
													(如果temp->next不为null)*/
		while(temp->next != NULL && temp->next->element.vertex < to)
			temp = temp->next;   /*确定边(from,to)或<from,to>在边表中的位置,如果不存在,
									则边(from,to)或<from,to>为新加的一条边*/
		if(temp->next == NULL) {  /*边(from,to)或<from,to>在边表中不存在且在边表中其后
									已无其它边,则在边表中加入这条边*/
			temp->next = new Link<listUnit>;
			temp->next->element.vertex = to;
			temp->next->element.weight = weight;
			numEdge++;
			Indegree[to]++;
			return;
		}
		if(temp->next->element.vertex == to)  { /*边(from,to)或<from,to>在边表中已存在,
												故只需要改变边的权值*/
			temp->next->element.weight = weight;
			return;
		}
		if(temp->next->element.vertex>to) { /*边(from,to)或<from,to>在边表中不存在,但在边表中
												其后存在其它边,则在边表中插入这条边*/
			Link<listUnit> *other = temp->next;
			temp->next = new Link<listUnit>;
			temp->next->element.vertex = to;
			temp->next->element.weight = weight;
			temp->next->next = other;
			numEdge++;
			Indegree[to]++;
			return;
		}
	}

	void delEdge(int from,int to)  {          //删掉图的一条边
		Link<listUnit> *temp = graList[from].head;  /*graList[from].head保存的是顶点from的边表，temp->next指向顶点from的第一条边(如果temp->next不为null)*/
		while(temp->next != NULL && temp->next->element.vertex<to)  
			temp = temp->next;	/*确定边(from,to)或<from,to>在边表中的位置(如果该边存在)*/
		if(temp->next == NULL)
			return;        //边(from,to)或<from,to>在边表中不存在,则不需要做任何操作
		if(temp->next->element.vertex>to)
			return;        //边(from,to)或<from,to>在边表中不存在,则不需要做任何操作
		if(temp->next->element.vertex == to) { /*边(from,to)或<from,to>在边表中存在且确
											   定了该边在边表中的位置,则从边表中将其删掉*/
			Link<listUnit> *other = temp->next->next;
		    delete temp->next;
		    temp->next = other;
		    numEdge--;
		    Indegree[to]--;
		    return;
		}
	}

	void IniGraphl(Graphl *Graphl, int A[N][N]);
	void DFS(Graph& G, int v);
	void BFS(Graph& G, int v);
	void Visit(Graph &G, int v);
};

// 函数功能：初始化图
void Graphl::IniGraphl(Graphl *Graphl, int A[N][N])  {
	for (int i = 0; i < N; i ++)  	{
		for (int j = 0; j < N; j ++)  {
			if (A[i][j] > 0)
				Graphl->setEdge(i, j, A[i][j]);
		}
	}
}


//函数功能：深度优先搜索算法实现
void Graphl::DFS(Graph& G, int v)  {    //深度优先搜索算法实现
    G.Mark[v] = VISITED;       //访问顶点v，并标记其标志位
    Visit(G,v);	
    //访问V邻接到的未被访问过的顶点，并递归地按照深度优先的方式进行周游
    for(Edge e=G.FirstEdge(v);G.IsEdge(e);e=G.NextEdge(e))
        if(G.Mark[G.ToVertex(e)]== UNVISITED)
			DFS(G, G.ToVertex(e));
}


// 函数功能：广度优先搜索
void Graphl::BFS(Graph& G, int v)  {           // 广度优先搜索算法的实现
    using std::queue;
	queue<int> Q;                    // 初始化广度优先周游要用到的队列
	Visit(G,v);                       // 问顶点v，并标记其标志位
    G.Mark[v] = VISITED;
    Q.push(v);                       // v入队
	while (!Q.empty())  {              // 如果队列仍然有元素
		int u = Q.front ();              // 队列顶部元素
		Q.pop();                     // 队列顶部元素出队
        // 该顶点邻接到的每一个未访问顶点都入队
		for (Edge e = G.FirstEdge(u);G.IsEdge(e);e = G.NextEdge(e))
		if (G.Mark[G.ToVertex(e)] == UNVISITED)  {
		        Visit(G, G.ToVertex(e));	
				G.Mark[G.ToVertex(e)] = VISITED;
		    	Q.push(G.ToVertex(e));  // 入队
		    }
	}
}


// 函数功能：显示顶点
void Graphl::Visit(Graph &G, int v) {
	cout << 'V' << v <<" ";
}
