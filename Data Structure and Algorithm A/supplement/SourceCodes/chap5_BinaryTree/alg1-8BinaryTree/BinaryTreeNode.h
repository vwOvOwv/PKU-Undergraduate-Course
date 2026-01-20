/*  BinaryTreeNode.h  */


template <class T> class BinaryTree;

template <class T>
class BinaryTreeNode {
friend class BinaryTree<T>;	//声明二叉树为结点类的友元类，便于访问私有数据成员
private:
	T  info;				     	    	//二叉树结点数据域
	BinaryTreeNode<T>* left;		   		//二叉树结点指向左子树的指针
	BinaryTreeNode<T>* right;    			//二叉树结点指向左子树的指针

public:
	BinaryTreeNode();							//缺省构造函数
	BinaryTreeNode(const T& inf);	 			//给定数据的构造函数
	BinaryTreeNode(const T& inf,BinaryTreeNode<T>* l, BinaryTreeNode<T>* r);//给定了结点值和左右子树的构造函数
	T  value() const;							//返回当前结点的数据
	BinaryTreeNode<T>*  leftchild() const;		//返回当前结点左子树
	BinaryTreeNode<T>*  rightchild() const;		//返回当前结点右子树
	void  setLeftchild(BinaryTreeNode<T>*) ;	//设置当前结点的左子树
	void  setRightchild(BinaryTreeNode<T>*) ;	//设置当前结点的右子树
	void  setValue(const T& val); 				//设置当前结点的数据域
	bool  isLeaf() const;				//判定当前结点是否为叶结点,若是返回true
	BinaryTreeNode<T>& operator = (const BinaryTreeNode<T>& Node){this = Node;};//重载赋值操作符
};


//****** BinaryTreeNode Implementation *******//

template<class T>
BinaryTreeNode<T>::BinaryTreeNode()  {
	left = right = NULL;
}

template<class T>
BinaryTreeNode<T>::BinaryTreeNode(const T& inf)  {	//给定数据的构造函数
	info = inf;
	left = right = NULL;
}

template<class T>
BinaryTreeNode<T>::BinaryTreeNode(const T& inf,BinaryTreeNode* l, BinaryTreeNode* r)  {//给定数据的左右指针的构造函数
	info = inf;
	left = l;
	right = r;
}

template<class T>
T  BinaryTreeNode<T>::value() const  {
	return info; 
}	

template<class T>
BinaryTreeNode<T>*  BinaryTreeNode<T>::leftchild() const  { //返回当前结点指向左子树的指针
	return left;
}												

template<class T>
BinaryTreeNode<T>*  BinaryTreeNode<T>::rightchild() const  { //返回当前结点指向右子树的指针
	return right;								
}			

template<class T>
void  BinaryTreeNode<T>::setLeftchild(BinaryTreeNode<T>* subroot)  { //设置当前结点的左子树
	left = subroot;
}

template<class T>
void  BinaryTreeNode<T>::setRightchild(BinaryTreeNode<T>* subroot)  { //设置当前结点的右子树
	right = subroot;
}

template<class T>
void  BinaryTreeNode<T>::setValue(const T& val)  {	//设置当前结点的数据域
	info = val; 
} 									

template<class T>
bool  BinaryTreeNode<T>::isLeaf() const	 {	//判定当前结点是否为叶结点,若是返回true
	return (left == NULL) && (right == NULL); 
}



