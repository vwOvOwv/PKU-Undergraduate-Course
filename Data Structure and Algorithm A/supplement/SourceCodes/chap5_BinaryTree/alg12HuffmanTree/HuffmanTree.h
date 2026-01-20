// -------- HuffmanTree.h

#include "MinHeap.h"
#include <iostream>
using namespace std;

template <class T> class HuffmanTree;
template <class T>
class HuffmanTreeNode  {
	friend class HuffmanTree<T>;
private:
	T info;
	HuffmanTreeNode<T> *parent;
	HuffmanTreeNode<T> *left;
	HuffmanTreeNode<T> *right;	
public:
	HuffmanTreeNode() {};
	HuffmanTreeNode<T> *leftchild() {return left;};
	HuffmanTreeNode<T> *rightchild() {return right;};
	bool operator > (HuffmanTreeNode<T> &HN) {return info > HN.info;}; // 注意要重载运算符
	bool operator < (HuffmanTreeNode<T> &HN) {return info < HN.info;}; 
	bool operator == (HuffmanTreeNode<T> &HN) {return info == HN.info;};
};

template <class T>
class HuffmanTree  {
private:
	HuffmanTreeNode<T>* root;					//Huffman树的树根

	//把ht1和ht2为根的Huffman子树合并成一棵以parent为根的二叉树
	void MergeTree ( HuffmanTreeNode<T> &ht1, HuffmanTreeNode<T> &ht2,  HuffmanTreeNode<T>* parent);
	
	void DeleteTree(HuffmanTreeNode<T>* root);//删除Huffman树或其子树
public:
	//构造Huffman树，weight是存储权值的数组，n是数组长度
	HuffmanTree(T weight[],int n);	
	virtual ~HuffmanTree(){	DeleteTree(root);}; 	//析构函数

	void InOrder(HuffmanTreeNode<T> *root);          //中序周游 
	HuffmanTreeNode<T> *GetRoot() {return root;};

};

template<class T>
HuffmanTree<T>::HuffmanTree(T weight[], int n)  {
	MinHeap< HuffmanTreeNode<T> > heap(n);		//定义最小值堆
	HuffmanTreeNode<T> *parent, firstchild, secondchild;
	HuffmanTreeNode<T>* NodeList = new HuffmanTreeNode<T>[n];
	for(int i = 0;i < n;i++)  {    				//初始化
		NodeList[i].info = weight[i];
		NodeList[i].parent = NodeList[i].left = NodeList[i].right = NULL;
		heap.Insert(NodeList[i]);				//向堆中添加元素
	}
	int i;
	for(i = 0;i < n-1;i++)  {               		//通过n-1次合并建立Huffman树
		parent = new HuffmanTreeNode<T>;
		firstchild = heap.RemoveMin();				//选择权值最小的结点
		secondchild = heap.RemoveMin();				//选择权值次小的结点
		MergeTree(firstchild,secondchild,parent);	//合并权值最小的两棵树
		heap.Insert(*parent);					//把parent插入到堆中去
		root = parent;							//建立根结点
	}
	delete []NodeList;
}

template <class T>
void HuffmanTree<T>::DeleteTree(HuffmanTreeNode<T> *root)  {
	if (root)  	{
		DeleteTree(root->left);
		DeleteTree(root->right);
		delete root;
	}
}


template <class T>
void HuffmanTree<T>::InOrder(HuffmanTreeNode<T> *root)  { //中序周游
	if (root)  	{
		InOrder(root->left);
		cout << root->info << " ";
		InOrder(root->right);
	}
}


template <class T>
void HuffmanTree<T>::MergeTree(HuffmanTreeNode<T> &ht1, HuffmanTreeNode<T> &ht2, HuffmanTreeNode<T> *parent)  {
	HuffmanTreeNode<T> *l = new HuffmanTreeNode<T>();
	HuffmanTreeNode<T> *r = new HuffmanTreeNode<T>();
	
	*l = ht1; // 不能写为l = &ht1，注意地址引用，开辟的是新空间，或者应用拷贝构造函数，只是有些麻烦
	*r = ht2;
	
	parent->parent = NULL;
	parent->left = l;
	parent->right = r;
	parent->info = ht1.info + ht2.info;
	ht1.parent = ht2.parent = parent; // 指向父节点
}

