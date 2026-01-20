//this is avl tree class
#include "AVLNode.h"
using namespace std;
template <class T> class avlTree//平衡二叉树类
{
	public:
		avlTree();//构造函数
		avlTree(const avlTree<T> &source);
		~avlTree();//析构函数
		void add(T value);
		void remove(T value);
		void deleteAllValue();
		void display();
		void display(avlNode<T>* found);
		avlNode<T>* findValue(T val);
	private:
		avlNode<T> *root;
};


template <class T>
avlTree<T>::avlTree():root(0)
{}//建立空树
template <class T>
avlTree<T>::avlTree(const avlTree<T> &source)
{//复制构造函数
	root=(source.root==NULL?NULL:source.root->copy());
}
template <class T> avlTree<T>::~avlTree()
{
	if (root!=NULL) deleteAllValue();
}
template <class T> void avlTree<T>::add(T value)
{//向平衡的avl树添加元素
	if(root==NULL)
		root=new avlNode<T>(value);
	else
		root->add(root,value);
}

template <class T> void avlTree<T>::remove(T val)
{//从avl树中删除一个元素
	avlNode<T>* eNode=NULL;
	int flag=0;
	if (root)
		root=root->remove(val,eNode,flag);
	if (eNode)
		delete eNode;
	else cout<<"The avlTree doesn't contain the value"<<endl;
}

template <class T> void avlTree<T>::deleteAllValue()
{//删除avl树的全部元素
	if(root!=NULL)
	{
		root->release();
		delete root;
		root=NULL;
	}
}
template <class T> void avlTree<T>::display()
{
	if (root) 
	{
		cout<<setw(8)<<"节点值"<<setw(10)<<"平衡因子"<<" "<<setw(6)<<"层数"<<endl;
		root->preorderview(root);
	}
	else cout<<"当前树空!"<<endl;
}
template <class T> avlNode<T>* avlTree<T>::findValue(T val)
{//查找val的节点值
	avlNode<T>* found=root->findNodeValue(val);
	if (found==NULL)
		return NULL;
	else
		return found;
}
template <class T> void avlTree<T>::display(avlNode<T>* found)
{//显示指针的指向的码值
	if(found==NULL) 
		cout<<"Sorry,没找到 :("<<endl;
	else
		cout<<"找到:"<<found->value<<endl;
}

