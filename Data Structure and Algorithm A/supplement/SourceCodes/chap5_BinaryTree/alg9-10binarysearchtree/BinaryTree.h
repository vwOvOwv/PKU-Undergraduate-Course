//************BinaryTree.h****************//

#include <stack>
#include <queue>
#include "BinaryTreeNode.h"

enum Tags{Left,Right};    //枚举类型

template <class T>
class StackElement  {         //StackElement
public:
	BinaryTreeNode<T>* pointer;
	Tags tag;
};	

using namespace std;

template <class T>
class BinaryTree {
protected:
	BinaryTreeNode<T>*  root;      			//二叉树根结点
public:
	BinaryTree(){root = NULL;};				//构造函数
	~BinaryTree() {DeleteBinaryTree(root);};	//析构函数
	bool isEmpty() const;						//判定二叉树是否为空树
	BinaryTreeNode<T>* Root(){return root;};	//返回二叉树根结点
	BinaryTreeNode<T>* Parent(BinaryTreeNode<T>* current);//返回current的父结点
	BinaryTreeNode<T>* LeftSibling(BinaryTreeNode<T>* current);
	//返回current结点的左兄弟
	BinaryTreeNode<T>* RightSibling(BinaryTreeNode<T>* current);
	//返回current结点的右兄弟
	void CreateTree(const T& info, BinaryTree<T>& leftTree, BinaryTree<T>& rightTree);
	//构造一棵以info为根、leftTree和rightTree为左右子树的新二叉树
	void PreOrder(BinaryTreeNode<T>* root);  	//前序周游二叉树或其子树
	void InOrder(BinaryTreeNode<T>* root);		//中序周游二叉树或其子树
	void PostOrder(BinaryTreeNode<T>* root);	//后序周游二叉树或其子树
	void PreOrderWithoutRecursion(BinaryTreeNode<T>* root);//非递归前序周游二叉树或其子树
	void InOrderWithoutRecursion(BinaryTreeNode<T>* root);//非递归中序周游二叉树或其子树
	void PostOrderWithoutRecursion(BinaryTreeNode<T>* root);//非递归后序周游二叉树或其子树
	void LevelOrder(BinaryTreeNode<T>* root); 	//按层次周游二叉树或其子树
	void DeleteBinaryTree(BinaryTreeNode<T>* root);	//删除二叉树或其子树
	void Visit(T Value) {cout << Value<< ", ";};           //访问
};


//**********  BianryTree Implementation  ***********//

template<class T>
bool BinaryTree<T>:: isEmpty() const  {      //判定二叉树是否为空树
	return ( root? false : true);
}

template<class T>
BinaryTreeNode<T>* BinaryTree<T>::Parent(BinaryTreeNode<T>* current)  {
	using std::stack;						//使用STL中的stack
	stack<BinaryTreeNode<T>* > aStack;
	BinaryTreeNode<T>* pointer = root;      	//保存输入参数	
	if(NULL != root && NULL != current)  {		
		while(!aStack.empty() || pointer)	{
			if (pointer)  {
				if(current == pointer->leftchild() ||current == pointer->rightchild()) //如果当前pointer的孩子就是current，返回parent
					return pointer;
				aStack.push(pointer);				//当前结点地址入栈
				pointer = pointer->leftchild();		//当前链接结构指向左孩子
			}
			else  {                         //左子树访问完毕，转向访问右子树
				pointer = aStack.top();			//栈顶元素退栈                 
				aStack.pop();
				pointer = pointer->rightchild();  	//当前链接结构指向右孩子
			}//endif
		} //endwhile
	}//endif
}


template<class T>
BinaryTreeNode<T>* BinaryTree<T>::LeftSibling(BinaryTreeNode<T>* current)  {
	//返回current结点的左兄弟
	if(current)  {
		BinaryTreeNode<T>* temp = Parent(current);    //返回current结点的父结点
		if ((temp == NULL) || current == temp->leftchild())
			return  NULL;	  //如果父结点为空，或者current没有左兄弟，返回空
		else return temp->leftchild();
	}
	return NULL;
}

template<class T>
BinaryTreeNode<T>* BinaryTree<T>::RightSibling(BinaryTreeNode<T>* current)  {
	//返回current结点的右兄弟
	if(current)  {
		BinaryTreeNode<T>* temp = Parent(current);//返回current结点的父结点
		if(temp == NULL||current == temp->rightchild())
			return  NULL;		    //如果父结点为空，或者current没有右兄弟
		else return temp->rightchild();
	}
	return NULL;
}

template<class T>
void BinaryTree<T>:: CreateTree (const T& info, BinaryTree<T>& leftTree, BinaryTree<T>& rightTree)  {
	//由左子树leftTree、右子树rightTree和数据元素info创建一棵新树，根结点是info
	//其中this、leftTree、rightTree必须是不同的三棵树
	root = new BinaryTreeNode<T>(info, leftTree.root, rightTree.root);	//创建新树
	leftTree.root = rightTree.root = NULL;  //原来两棵子树的根结点指空，避免访问
}

template<class T>
void BinaryTree<T>:: DeleteBinaryTree(BinaryTreeNode<T>* root)  { //以后序周游的方式删除二叉树
	if(root)  {
		DeleteBinaryTree(root->left);				//递归删除左子树
		DeleteBinaryTree(root->right);		    //递归删除右子树
		delete root;							//删除根结点
	}
}

template<class T>
void BinaryTree<T>::PreOrder (BinaryTreeNode<T>* root)  {  //前序周游二叉树
	if(root != NULL)  {
		Visit(root->value());						//访问当前结点
		PreOrder(root->leftchild());			//访问左子树
		PreOrder(root->rightchild());			//访问右子树
	}
}
template<class T>
void BinaryTree<T>:: InOrder (BinaryTreeNode<T>* root)  {  //中序周游二叉树
	if(root != NULL)  {
		InOrder (root->leftchild());			//访问左子树
		Visit(root->value());						//访问当前结点
		InOrder (root->rightchild());			//访问右子树
	}
}
template<class T>
void BinaryTree<T>:: PostOrder (BinaryTreeNode<T>* root)  { //后序周游二叉树
	if(root != NULL)  {
		PostOrder(root->leftchild());			//访问左子树
		PostOrder (root->rightchild());		//访问右子树
		Visit(root->value());						//访问当前结点
	}
}

template<class T>
void BinaryTree<T>::PreOrderWithoutRecursion(BinaryTreeNode<T>* root)  {//非递归前序周游二叉树或其子树
	using std::stack;						//使用STL中的stack
	stack<BinaryTreeNode<T>* > aStack;
	BinaryTreeNode<T>* pointer = root;      	//保存输入参数	
	while(!aStack.empty() || pointer)	{
		if (pointer)  {
			Visit(pointer->value());			//访问当前结点
			aStack.push(pointer);				//当前结点地址入栈
			pointer = pointer->leftchild();		//当前链接结构指向左孩子
		}
		else  {                         //左子树访问完毕，转向访问右子树
			pointer = aStack.top();			//栈顶元素退栈                 
			aStack.pop();
			pointer = pointer->rightchild();  	//当前链接结构指向右孩子
		}//endif
    } //endwhile
}

template<class T>
void BinaryTree<T>::InOrderWithoutRecursion(BinaryTreeNode<T>* root)  {
	//非递归中序周游二叉树或其子树
	using std::stack;							//使用STL中的stack
	stack<BinaryTreeNode<T>* > aStack;
	BinaryTreeNode<T>* pointer = root;      	//保存输入参数	
	while(!aStack.empty() || pointer)  {
		if (pointer)  {
			aStack.push(pointer);				//当前结点地址入栈
			pointer = pointer->leftchild();		//当前链接结构指向左孩子
		}
		else  {                            //左子树访问完毕，转向访问右子树
			pointer = aStack.top();
			aStack.pop();					//栈顶元素退栈 
			Visit(pointer->value());		//访问当前结点
			pointer = pointer->rightchild(); 	//当前链接结构指向右孩子
                
		}
	} //endwhile
}

template<class T>
void BinaryTree<T>::PostOrderWithoutRecursion(BinaryTreeNode<T>* root)  {
	//非递归后序周游二叉树或其子树
	using std::stack;							//使用STL栈部分
	StackElement<T> element;
	stack<StackElement<T > > aStack;			//栈申明
	BinaryTreeNode<T>* pointer;
	if(root == NULL)
		return;							//空树即返回
	else pointer = root;						//保存输入参数
	while (true)  {                        //进入左子树			
		while(pointer != NULL) 	{
			element.pointer = pointer;
			element.tag = Left;
			aStack.push(element);
			pointer = pointer->leftchild();		//沿左子树方向向下周游
		}	
		element = aStack.top();
		aStack.pop();                         //托出栈顶元素
		pointer = element.pointer;
		while(element.tag == Right)  {           //从右子树回来
			Visit(pointer->value());		    //访问当前结点
			if (aStack.empty())
				return;
			else  {
				element = aStack.top();
				aStack.pop();				//弹出栈顶元素
				pointer = element.pointer;
			}
		}
		//从左子树回来
		element.tag = Right;
		aStack.push(element);
		pointer = pointer->rightchild();				//转向访问右子树
	}
}

template<class T>
void BinaryTree<T>::LevelOrder(BinaryTreeNode<T>* root)	{
	//按层次周游二叉树或其子树
	using std::queue;							//使用STL的队列
	queue<BinaryTreeNode<T>*> aQueue;
	BinaryTreeNode<T>* pointer = root;			//保存输入参数
	if (pointer)
		aQueue.push(pointer);                  //根结点入队列
	while (!aQueue.empty())  {                 //队列非空
		pointer = aQueue.front();			 	//取队列首结点
		aQueue.pop();                        //当前结点出队列
        Visit(pointer->value());					//访问当前结点
		if(pointer->leftchild())
			aQueue.push(pointer->leftchild());		//左子树进队列
		if(pointer->rightchild())
			aQueue.push(pointer->rightchild());	//右子树进队列
	}
}