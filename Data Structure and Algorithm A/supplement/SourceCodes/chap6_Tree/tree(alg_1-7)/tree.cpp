#include <iostream>
#include <queue>

#include "TreeNode.h"
#include "Tree.h"

using namespace std;


// 函数功能：周游树，在这里只列举一种（先根次序）
void Traverse(Tree<char> *tree)
{
	cout << "FirstRoot traverse:  ";
	tree->RootFirstTraverse(tree->getRoot()); // 先根深度优先周游
	cout << endl;
	
	cout << "LastRoot  traverse:  ";
	tree->RootLastTraverse(tree->getRoot()); // 后根深度
	cout << endl;
	
	cout << "Width traverse    :  ";
	tree->WidthTraverse(tree->getRoot()); // 广度遍历
	cout << endl << endl;
}


int main()  {
	//建树
	Tree<char> aTree;
	aTree.CreateRoot('A');
	TreeNode<char> *f = new TreeNode<char>('F');
	TreeNode<char> *e = new TreeNode<char>('E');
	TreeNode<char> *d = new TreeNode<char>('D');
	TreeNode<char> *c = new TreeNode<char>('C');
	TreeNode<char> *b = new TreeNode<char>('B');
	e->setSibling(f);
	c->setSibling(d);
	c->setChild(e);
	b->setSibling(c);
	aTree.getRoot()->setChild(b);

	TreeNode<char> *x = new TreeNode<char>('X');
	TreeNode<char> *y = new TreeNode<char>('Y');
	TreeNode<char> *z = new TreeNode<char>('Z');
	aTree.getRoot()->setSibling(x);
	x->setChild(y);
	y->setSibling(z);

	
	//显示树的结构
	cout << "      A            X        \n";
    cout << "   /  |  \\      /   \\     \n";
    cout << "  B   C    D    Y     Z     \n";
    cout << "    /   \\                  \n";
    cout << "   E     F                  \n";
    cout << "\n";
	
	//判断树是否为空
	aTree.isEmpty();
	
	//周游树
	cout << "周游树：\n";
	Traverse(&aTree);
	
	
	//e结点插入第一个孩子G
	cout << "e结点插入第一个孩子G的结果：\n";
	TreeNode<char> *g = new TreeNode<char>('G');
	e->InsertFirst(g);
	//周游树
	Traverse(&aTree);
	
	//e结点以第一个兄弟的身份插入H
	cout << "e结点以第一个兄弟的身份插入H的结果:\n";
	TreeNode<char> *h = new TreeNode<char>('H');
	e->InsertNext(h);
	//周游树
	Traverse(&aTree);
	
	//获得C的父结点
	cout << "获得C的父结点： ";
	aTree.Visit(aTree.Parent(c)->Value());
	cout << endl;
	//获得H的上一个兄弟结点
	cout << "获得H的上一个兄弟结点： ";
	aTree.Visit(aTree.PrevSibling(h)->Value());
	cout << endl;
//	aTree.Visit(aTree.PrevSibling(e)->Value());
//	cout << endl;
	
	//求父结点
	cout<<"E的父结点：";
	aTree.Visit(aTree.Parent(e)->Value());
	cout<<endl;
	cout<<"D的父结点：";
	aTree.Visit(aTree.Parent(d)->Value());	
	cout<<endl;
	cout<<"F的父结点：";
	aTree.Visit(aTree.Parent(f)->Value());
	cout<<endl;

	cout<<"Z的父结点：";
	aTree.Visit(aTree.Parent(z)->Value());
	cout<<endl;

	cout<<"X的父结点：";
	if (aTree.Parent(x) == NULL)
		cout<< "Parent is NULL!" << endl;

	//删除根为E的子树
	cout << "删除E子树：" << endl;
	aTree.DeleteSubTree(e);
	Traverse(&aTree);

	//删除根为C的子树
	cout << "删除C子树：" << endl;
	aTree.DeleteSubTree(c);
	Traverse(&aTree);
	return 0;
}