//*******  BinarySearchTree.h  ******//

//#include "BinaryTreeNode.h"
#include "BinaryTree.h"


template <class T>
class BinarySearchTree:public BinaryTree<T>  {
public:
	BinarySearchTree(){this->root=NULL;};
	virtual ~BinarySearchTree(){};
	void Initialize(BinaryTreeNode<T> *newpointer);
	BinaryTreeNode<T>* Root() {return this->root;};
	
	void InsertNode(BinaryTreeNode<T>* root,BinaryTreeNode<T>* newpointer);
	void DeleteNode(BinaryTreeNode<T>* pointer);
	void DeleteNodeEx(BinaryTreeNode<T>* pointer);
};


//*********  implemention ***********//

template <class T>
void BinarySearchTree<T>::Initialize(BinaryTreeNode<T> *newpointer)
{
	this->root = newpointer;
}

template<class T>
void BinarySearchTree<T>::InsertNode(BinaryTreeNode<T>* root , BinaryTreeNode<T>* newpointer)  {                          //向二叉搜索树插入新结点
	BinaryTreeNode<T>* pointer = NULL;
	if(root == NULL)  {                 //如果是空树，初始化
		Initialize(newpointer);	        //用指针newpointer初始化二叉搜索树树根，赋值实现
		return;
	}
	else 
		pointer = root;
	while(1)  {
		if (newpointer->value() == pointer->value())
			return ;					  //相等则不用插入
		else if (newpointer->value() < pointer->value())  {
			if(pointer->leftchild() == NULL)  {
				pointer->left = newpointer;		//作为左子树
				return;
			}
			else	pointer = pointer->leftchild();
		}
		else  {
			if(pointer->rightchild() == NULL)  {
				pointer->right = newpointer;      //作为右子树
				return;
			}
			else	pointer = pointer->rightchild();
		}
	}//endwhile
}

/*
template<class T>
void BinarySearchTree<T>::DeleteNode(BinaryTreeNode<T>* pointer)  {      //教材中写出这个算法
	//二叉搜索树中结点的删除
	BinaryTreeNode<T>* temppointer = NULL;
	if(!pointer)                            //如果删除的是空结点，则返回
		return;
	BinaryTreeNode<T>* parent = Parent(pointer);
	if (pointer->leftchild() == NULL)  {
		//被删结点无左子树，则将其右子树的根代替该删除结点
		if (parent == NULL)                 //被删除结点是根结点
			root = pointer->rightchild();
		else if (parent->leftchild() == pointer)
			parent->left = pointer->rightchild();
		else
			parent->right = pointer->rightchild();
		delete pointer;                     //删除该结点
		pointer = NULL;
		return;
	}
	else                                 //左子树不为空时
		temppointer = pointer->leftchild();
	while (temppointer->rightchild() != NULL)  //在左子树中找对称序的最后一个结点
		temppointer = temppointer->rightchild();
	//被删除结点的右子树作为temppointer的右子树
	temppointer->right = pointer->rightchild();
	//被删除结点的左子树根代替被删除结点
	if(NULL == parent)
		root = pointer->leftchild();
	else if (parent->leftchild() == pointer)
		parent->left = pointer->leftchild();
	else 
		parent->right = pointer->leftchild();
	delete pointer;                          //删除该结点
	pointer = NULL;
	return;
}
*/

template <class T>
void BinarySearchTree<T>::DeleteNodeEx(BinaryTreeNode<T>* pointer)  {
	if ( pointer == NULL )                    // 若待删除结点不存在，返回
		return;
	BinaryTreeNode<T> * temppointer;         // 用于保存替换结点
	BinaryTreeNode<T> * tempparent = NULL;  // 用于保存替换结点的父结点
	BinaryTreeNode<T> * parent = Parent(pointer ); // 保存删除结点的父结点
	// 如果待删除结点的左子树为空，就将它的右子树代替它
	if ( pointer->leftchild() == NULL )  
       temppointer = pointer->rightchild();
    else {
		// 当待删除结点左子树不为空，就在左子树中寻找最大结点替换待删除结点
		temppointer = pointer->leftchild();
		while (temppointer->rightchild() != NULL )  {
			tempparent = temppointer;
			temppointer = temppointer->rightchild();
		}
		// 删除替换结点
		if (tempparent == NULL)
			pointer->left = temppointer->leftchild();
		else tempparent->right = temppointer->leftchild();
		temppointer->left = pointer->leftchild();    // 继承pointer的左子树
		temppointer->right = pointer->rightchild();  // 继承pointer的右子树
	}
	// 用替换结点去替代真正的删除结点
	if (parent == NULL)
		this->root = temppointer;
	else if ( parent->leftchild() == pointer )
	    parent->left = temppointer;
	else parent->right = temppointer;
	delete pointer;                         	// 删除该结点
	pointer = NULL;
	return;
}



