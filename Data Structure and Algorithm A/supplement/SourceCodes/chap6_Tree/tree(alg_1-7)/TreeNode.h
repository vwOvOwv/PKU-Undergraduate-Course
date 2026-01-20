template  <class T> class Tree;        //声明树类

template  <class T>
class TreeNode  {
friend class Tree<T>;
private:
	T m_Value;								//树结点的值
	TreeNode<T>*	pChild;					//左子结点
	TreeNode<T>*	pSibling;				//右兄弟结点
public:
	TreeNode(const T& value);			 //拷贝构造函数
	virtual ~TreeNode() {};			     //析构函数
	bool isLeaf();					     //如果结点是叶，返回true
	T Value();							 //返回结点的值 
	TreeNode<T>* LeftMostChild();		 //返回第一个左孩子
	TreeNode<T>* RightSibling();		 //返回右兄弟
	void setValue(T&);					 //设置结点的值 
	void setChild(TreeNode<T>* pointer);	 //设置左孩子
	void setSibling(TreeNode<T>* pointer);   //设置右兄弟
	void InsertFirst(TreeNode<T>* node);	 //以第一个左孩子身份插入结点
	void InsertNext(TreeNode<T>* node);	 //以右兄弟的身份插入结点
};

template<class T>
TreeNode<T>::TreeNode(const T& value)	{	//拷贝构造函数
	m_Value=value;
	pChild=NULL;
	pSibling=NULL;
}

template  <class T>
bool TreeNode<T>::isLeaf()  {
    //如果结点是叶，返回true
	if (pChild == NULL)
		return true;
	return false;
}

template<class T>
T TreeNode<T>::Value()	{					//返回结点的值
	return m_Value;
}

template<class T>
TreeNode<T>* TreeNode<T>::LeftMostChild()  { //返回第一个左孩子
	return pChild;
}

template<class T>
TreeNode<T>* TreeNode<T>::RightSibling()  {	//返回右兄弟
	return pSibling;
}

template  <class T>
void TreeNode<T>::setValue(T& value)  {
    //设置结点的值
	m_Value = value;
}

template<class T>
void TreeNode<T>::setChild(TreeNode<T>* pointer)  {  //设置左孩子
	pChild=pointer;
}

template<class T>
void TreeNode<T>::setSibling(TreeNode<T>* pointer)  {  //设置右兄弟
	pSibling=pointer;
}

template  <class T>
void TreeNode<T>::InsertFirst(TreeNode<T>* node)  {
    //以第一个孩子的身份插入结点
	if (pChild)
			node->pSibling = pChild;
	pChild = node;
}

template<class T>
void TreeNode<T>::InsertNext(TreeNode<T>* node)  { //以右兄弟的身份插入结点
	if(pSibling)		
		node->pSibling=pSibling;
	pSibling=node;
}






