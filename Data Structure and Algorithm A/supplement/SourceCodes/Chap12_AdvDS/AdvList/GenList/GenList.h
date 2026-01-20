//广义表的链表实现
#include <iostream>
using namespace std;
#define ATOM 0  //表中存储的是原子元素
#define LIST 1  //表中存储的是子表
#define HEAD -1  //是否是表头结点
#define VISITED 1  //已经访问过
#define UNVISITED 0  //未访问过
#define BACKTRACE 1  //需要回溯
#define NOTBACKTRACE 0  //不需要回溯
#define MaxName 20; 

//广义表的结点类型
template <class T>
class GenListNode
{
	public:
		int type;   //表示该结点是ATOM or LIST	
		union {
			struct {
		         int ref;   //如果是表头结点则，存储该结点被几个结点所指向
				 char* Name;  // 表头名称	
				 int mark;   // 本子表是否被访问过
			} headNode;
	        GenListNode<T> *child;//如果是LIST ,则指向它的元素的首结点	
	        T element; //如果是ATOM，则存储它的值
		};

	    GenListNode<T> *next;//指向下一个结点
		GenListNode(T element);//构造函数
		GenListNode();
		GenListNode(GenListNode<T> *list);//
	   ~GenListNode();//析构函数
	    void Destory(GenListNode<T>* node);//释放所有内存 
	    void ViewAll();//遍历该结点的子孙和兄弟
	    void GenListTraversal();//遍历该结点的子孙和兄弟，张铭
		void Search(GenListNode<T>* node,GenListNode<T>*prenode,int &flag);
		void GenListTraversalHelp(GenListNode<T> *node);  // 张铭
};
//GenList是一个原子元素类型为T的广义表
//如果要实现能够存储多种数据类型的广义
//表，只需要嵌套的使用它

template <class T>
class GenList
{
	private:
		GenListNode<T> *head;//整个广义表的头结点,不存储任何信息
		GenListNode<T> *current;//当前指针的位置
	
	public:
		GenList(char *Name);
		~GenList();
		void Insert(T element);//在尾部加入一个元素结点
		void Insert(GenList<T> *gl);//在尾部插入一个子表
        GenListNode<T> *GetHead();//得到头结点的指针
		GenListNode<T> *GetNext();//得到当前结点的下一个结点
		GenListNode<T> *GetPrev();//得到当前结点的前一个结点
		void MoveToFirst();//当前指针指向head
		int Remove();//删除当前结点
		void View();//遍历广义表，伍赛
		void ViewList();//遍历广义表, 张铭
};

