//类ParTreeNode描述了树的结点定义
template<class T>
class ParTreeNode  { 					    //树结点定义
private:
	T				value;					//结点的值
	ParTreeNode<T>* parent;					//父结点指针
	int		nCount;							//以此结点为根的子树的总结点个数
public:
	ParTreeNode();							//构造函数
	virtual ~ParTreeNode(){};				//析构函数
	T	getValue();							//返回结点的值
	void  setValue(const T& val);			//设置结点的值
	ParTreeNode<T>*  getParent();			//返回父结点指针
	void  setParent(ParTreeNode<T>* par);	//设置父结点指针
	int   getCount();						//返回结点数目
	void  setCount(const int count);		//设置结点数目
};

//ParTreeNode抽象数据类型成员函数的实现
template<class T>
ParTreeNode<T>::ParTreeNode()  {            //构造函数
	parent = NULL;
	nCount = 1;
}
template<class T>
T ParTreeNode<T>::getValue()  {				//返回结点的值
	return value;
}
template<class T>
void ParTreeNode<T>::setValue(const T& val)  {			//设置结点的值
	value = val;
}
template<class T>
ParTreeNode<T>* ParTreeNode<T>::getParent()  {			//返回父结点指针
	return parent;
}
template<class T>
void ParTreeNode<T>::setParent(ParTreeNode<T>* par)  {   //设置父结点指针
	parent = par;
}
template<class T>
int ParTreeNode<T>::getCount()  {						//返回结点数目
	return nCount;
}
template<class T>
void ParTreeNode<T>::setCount(const int count)  {		//设置结点数目
	nCount = count;
}

//类ParTree描述了树的定义
template<class T>
class ParTree  {								//树定义
public:
	ParTreeNode<T>* array;						//存储树结点的数组
	int	Size;									//数组大小
	ParTree(const int size);					//构造函数
	virtual ~ParTree();							//析构函数
	ParTreeNode<T>*	Find(ParTreeNode<T>* node)const;//查找node结点的根结点
	ParTreeNode<T> *FindPC(ParTreeNode<T> *node) const; // 带压缩
	
	void Union(int i,int j);					//把下标为i，j的结点合并成一棵子树
	void UnionPC(int i, int j);                 //带压缩
	bool Different(int i,int j);				//判定下标为i，j的结点是否在一棵树中
};

//树的成员函数的实现
template <class T>
ParTree<T>::ParTree(const int size)  {			//构造函数
	Size = size;
	array = new ParTreeNode<T>[size];
}

template <class T>											
ParTree<T>::~ParTree()  {						//构造函数
	delete []array;
}

// 函数功能：找到目标结点的根结点
template <class T>
ParTreeNode<T>*	ParTree<T>::Find(ParTreeNode<T>* node) const  {
	ParTreeNode<T>* pointer = node;
	while(pointer->getParent() != NULL)
		pointer = pointer->getParent();
	return pointer;
}

//函数功能：带路径压缩的Find算法
template <class T>
ParTreeNode<T> *ParTree<T>::FindPC(ParTreeNode<T> *node) const  {
	if (node->getParent() == NULL)
		return node;
	
	node->setParent(FindPC(node->getParent()));
	return node->getParent();
}

//函数功能：判定下标为i，j的结点是否在一棵树中
template<class T>
bool ParTree<T>::Different(int i,int j)  {
	ParTreeNode<T>* pointeri = Find(&array[i]);		//找到结点i的根
	ParTreeNode<T>* pointerj = Find(&array[j]);		//找到结点j的根
	return pointeri != pointerj;
}

//把下标为i，j的结点合并成一棵子树
template<class T>
void ParTree<T>::Union(int i,int j)  {
	ParTreeNode<T>* pointeri = Find(&array[i]);		//找到结点i的根
	ParTreeNode<T>* pointerj = Find(&array[j]);		//找到结点j的根
	if(pointeri != pointerj) {
		if(pointeri->getCount()>=pointerj->getCount()) {
			pointerj->setParent(pointeri);
			pointeri->setCount(pointeri->getCount()+pointerj->getCount());
		}
		else {
			pointeri->setParent(pointerj);
			pointerj->setCount(pointeri->getCount()+pointerj->getCount());
		}
	}
}

// 函数功能：归并两个集合， 带压缩
template <class T>
void ParTree<T>::UnionPC(int i, int j)  {
	ParTreeNode<T> *pointeri = FindPC(&array[i]);
	ParTreeNode<T> *pointerj = FindPC(&array[j]);
	
	if (pointeri != pointerj)  {
		if (pointeri->getCount() >= pointerj->getCount())  {
			pointerj->setParent(pointeri);
			pointeri->setCount(pointeri->getCount() + pointerj->getCount());
		}
		else  {
			pointeri->setParent(pointerj);
			pointerj->setCount(pointeri->getCount() + pointerj->getCount());
		}
	}
}