//***************************//
#include <iostream>
#include "BinaryTree.h"


void main() {
	//��һ����(��ͼ5.5��ʾ)
	BinaryTree<char> a, b, c, d, e, f, g, h, i,nulltree;
	d.CreateTree('D', nulltree, nulltree);
	g.CreateTree('G', nulltree, nulltree);
	h.CreateTree('H', nulltree, nulltree);
	i.CreateTree('I', nulltree, nulltree);
	f.CreateTree('F', h, i);
	e.CreateTree('E', g, nulltree);
	b.CreateTree('B', d, e);
	c.CreateTree('C', nulltree, f);
	a.CreateTree('A', b, c);
	
	//ǰ�����ζ�����
	cout << "Preorder sequence is: "<<endl;
	a.PreOrder(a.Root());				//�ݹ�
	cout << endl;
	cout << "Preorder sequence Without Recursion is: " <<endl;
	a.PreOrderWithoutRecursion(a.Root());//�ǵݹ�
	cout << endl;

	//�������ζ�����
	cout << "Inorder sequence is: "<<endl;
	a.InOrder(a.Root());			//�ݹ�
	cout << endl;
	cout << "Inorder sequence Without Recursion is: " <<endl;
	a.InOrderWithoutRecursion(a.Root());//�ǵݹ�
	cout << endl;

	//�������ζ�����
	cout << "Postorder sequence is: "<<endl;
	a.PostOrder(a.Root());			//�ݹ�
	cout << endl;
	cout << "Postorder sequence Without Recursion is: " <<endl;
	a.PostOrderWithoutRecursion(a.Root());//�ǵݹ�	
	cout << endl;	

	//root
	cout << "Root is: " << a.Root()->value() <<endl;

/*	//delete tree
	a.DeleteBinaryTree(a.Root());
	cout<<"Tree is deleted."<<endl;        //û�����⣬�����������е���
*/

}