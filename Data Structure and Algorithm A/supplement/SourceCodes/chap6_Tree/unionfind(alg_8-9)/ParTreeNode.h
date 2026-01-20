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