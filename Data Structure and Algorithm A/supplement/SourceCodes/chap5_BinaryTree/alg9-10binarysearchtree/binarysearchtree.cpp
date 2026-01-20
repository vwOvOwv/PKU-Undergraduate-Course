#include <iostream>
#include "BinarySearchTree.h"

#define N 11

void main()  {
	int K[N] = {50,19,35,55,20,5,100,52,88,53,92};        //建一棵图5.11所示的树
	BinarySearchTree<int> aBST;
	BinaryTreeNode<int > *newpointer, *node1, *node2;     // 循环插入结点
	
	for (int i = 0; i < N; i ++)  {                       
		newpointer = new BinaryTreeNode<int>();
		newpointer->setValue(K[i]);
		aBST.InsertNode(aBST.Root(), newpointer);      //依次插入结点
		
		if (K[i] == 52){                                //记录待删除结点的指针node1
			node1 = newpointer;
			//	 cout<<node1->value()<<endl;
		}
		if (K[i] == 55)                                 //记录待删除结点的指针node2
			node2 = newpointer; 
	}
	
	// 遍历结点
	cout << "中序周游二叉搜索树：" << endl;
	aBST.InOrder(aBST.Root());
	cout << endl; 
	
	// 删除两个结点                          //教材中没讲这个算法             
	//  cout << "删除结点52, 55." << endl;
	//	BST.DeleteNode(node1);
	//	aBST.DeleteNode(node2); 
	
	// 删除两个结点的改进算法
	cout << "删除结点52, 55." << endl;
	aBST.DeleteNodeEx(node1);
	aBST.DeleteNodeEx(node2);
	
	// 遍历节点
	cout << "删除结点之后中序周游二叉搜索树：" << endl;	
	aBST.InOrder(aBST.Root());
	cout << endl;

}